#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>

using namespace std;

// Callback function to write data received from curl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to fetch the content of a webpage using libcurl
string fetchWebPage(const string& url) {
    CURL* curl;
    CURLcode res;
    string data;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return "";
        }
    }
    return data;
}

// Function to extract URLs from the page content
vector<string> extractURLs(const string& htmlContent) {
    vector<string> urls;
    regex urlRegex(R"(<a\s+(?:[^>]*?\s+)?href="([^"]*)")"", regex::icase);
    smatch urlMatches;
    string::const_iterator searchStart(htmlContent.cbegin());

    while (regex_search(searchStart, htmlContent.cend(), urlMatches, urlRegex)) {
        urls.push_back(urlMatches[1]);
        searchStart = urlMatches.suffix().first;
    }

    return urls;
}

int main() {
    string url;
    cout << "Enter the URL of the web page: ";
    getline(cin, url);

    // Fetch the web page content
    string content = fetchWebPage(url);
    if (content.empty()) {
        cerr << "Failed to fetch content from the URL." << endl;
        return 1;
    }

    // Extract the URLs from the page content
    vector<string> urls = extractURLs(content);

    // Display the extracted URLs
    cout << "\nList of URLs found on the page:" << endl;
    for (const string& extractedUrl : urls) {
        cout << extractedUrl << endl;
    }

    return 0;
}
