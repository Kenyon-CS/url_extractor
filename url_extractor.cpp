#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>

using namespace std;

// Write callback for libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static string ensureScheme(string url) {
    if (url.rfind("http://", 0) != 0 && url.rfind("https://", 0) != 0) {
        url = "https://" + url;
    }
    return url;
}

string fetchWebPage(const string& rawUrl, long &httpCodeOut) {
    string url = ensureScheme(rawUrl);
    CURL* curl = curl_easy_init();
    string data;
    httpCodeOut = 0;

    if (!curl) {
        cerr << "Failed to init CURL\n";
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, ""); // enable gzip/deflate
    curl_easy_setopt(curl, CURLOPT_USERAGENT,
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
        return "";
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCodeOut);
    curl_easy_cleanup(curl);
    return data;
}

// Extract URLs (matches href="...", href='...', href=unquoted)
vector<string> extractURLs(const string& htmlContent) {
    vector<string> urls;

    // Use a custom raw-string delimiter: R"re(... )re"
    const regex urlRegex(
        R"re(<a\b[^>]*\bhref\s*=\s*(?:"([^"]*)"|'([^']*)'|([^\s"'=<>`]+)))re",
        regex::ECMAScript | regex::icase);

    auto begin = sregex_iterator(htmlContent.begin(), htmlContent.end(), urlRegex);
    auto end   = sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        const smatch& m = *it;
        if (m[1].matched)      urls.push_back(m[1].str());
        else if (m[2].matched) urls.push_back(m[2].str());
        else if (m[3].matched) urls.push_back(m[3].str());
    }

    return urls;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    string url;
    cout << "Enter the URL of the web page: ";
    getline(cin, url);

    long httpCode = 0;
    string content = fetchWebPage(url, httpCode);
    if (content.empty()) {
        cerr << "Failed to fetch content from the URL.\n";
        curl_global_cleanup();
        return 1;
    }
    if (httpCode < 200 || httpCode >= 300) {
        cerr << "Warning: HTTP status " << httpCode << ". Parsing anyway...\n";
    }

    vector<string> urls = extractURLs(content);

    cout << "\nList of URLs found on the page:\n";
    for (const auto& u : urls) {
        cout << u << '\n';
    }
    if (urls.empty()) {
        cerr << "\n(No <a href=...> links matched.) First 400 chars of response:\n"
             << content.substr(0, 400) << "\n";
    }

    curl_global_cleanup();
    return 0;
}
