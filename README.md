# url_extractor
A C++ program that asks for a URL and returns a list of all the URLs that the page refers to.

## Explanation:
1. `WriteCallback`:
This is the callback function used by libcurl to collect data into a string as it is downloaded from the web page.
1. `fetchWebPage`:
Uses libcurl to download the HTML content of the specified URL and return it as a string.
1. `extractURLs`:
Uses a regular expression (regex) to find all URLs in <a href="..."> tags. It extracts the URL part and stores it in a vector of strings.
1. `main`:
Asks the user for a URL, fetches the content, extracts the URLs, and then prints the list of extracted URLs.

## Dependencies:
  - `libcurl`: You need to have libcurl installed to fetch the web page content.
  - `C++11` or later: We use regex which requires C++11 or later.

## Building and Running the Program
macOS and Linux:
1. Install libcurl:
For macOS: Use Homebrew to install libcurl.
```
brew install curl
```
For Linux: Use the package manager to install libcurl.
```
sudo apt-get install libcurl4-openssl-dev
```

Windows (with MinGW or Visual Studio):
1. Install libcurl:
Download pre-built binaries for Windows from the libcurl website.
2. Set up the build environment:
If using Visual Studio, set up a project and link it with the downloaded libcurl library.
If using MinGW, modify the Makefile to point to the correct paths for libcurl.
3. Compile and run:
For MinGW, compile with:
```
g++ -std=c++11 -o url_extractor url_extractor.cpp -lcurl
```
2. Run the program:
```
./url_extractor.exe
```
