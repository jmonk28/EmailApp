#include "EmailFunctionality.h"
#include "Utilities.h"
#include "Resource.h"
#include "framework.h"
#include <curl/curl.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include <Windows.h>
#include <shellapi.h>
#include <iostream>
#include <stdexcept>

// This function builds the authorization url to request a code from the OAuth2 endpoint
std::string getAuthCode() {
	// Grab client id from yaml file to use in auth request
	YAML::Node yaml_app = YAML::LoadFile("application.yaml");
	std::string client_id = yaml_app["client_id"].as<std::string>();

	// Define other auth parameters
	std::string redirect_uri = "http://127.0.0.1:8080/";
	std::string scope = "https://mail.google.com/";
	std::string response_type = "code";

	// Build the authorization URL
	std::string auth_url = "https://accounts.google.com/o/oauth2/v2/auth?"
		"client_id=" + client_id +
		"&redirect_uri=" + redirect_uri +
		"&response_type=" + response_type +
		"&scope=" + scope +
		"&access_type=offline";

	// Open the user's browser at the specified URL
	openBrowser(auth_url);

	// Grab code from redirect uri
	std::string code = listenForAuthRedirect();

	return code;
}

// This function opens the user's browser at the specified URL
void openBrowser(const std::string& url) {
	ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// This function uses WinSockets to listen for and grab the return code at the default endpoint (127.0.0.1:8080)
std::string listenForAuthRedirect() {
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, hints;
    char recvbuf[512];
    int iResult;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo("127.0.0.1", "8080", &hints, &result);
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    listen(ListenSocket, SOMAXCONN);

    std::cout << "Listening for OAuth redirect on http://127.0.0.1:8080...\n";

    ClientSocket = accept(ListenSocket, NULL, NULL);

    iResult = recv(ClientSocket, recvbuf, 511, 0);
    recvbuf[iResult] = '\0';
    std::string request(recvbuf);

    // Extract "code" parameter from GET request
    size_t pos = request.find("code=");
    std::string code;
    if (pos != std::string::npos) {
        size_t end = request.find(' ', pos);
        code = request.substr(pos + 5, end - (pos + 5));
    }

    // Send simple HTTP response back to browser
    const char* httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><body><h1>Authorization successful!</h1>"
        "You may close this window.</body></html>";
    send(ClientSocket, httpResponse, (int)strlen(httpResponse), 0);

    closesocket(ClientSocket);
    closesocket(ListenSocket);
    WSACleanup();

    return code;
}

// This function uses the obtained code to request an access token
bool getAccessToken(std::string& response) {
    // Make a curl instance and response instance and initialize
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl) {
        throw std::runtime_error("Could not create curl instance");
    }

    YAML::Node yaml_app = YAML::LoadFile("application.yaml");
    // Define request parameters
    std::string auth_code = getAuthCode();
    std::string client_id = yaml_app["client_id"].as<std::string>();
    std::string client_secret = yaml_app[client_secret].as<std::string>();
    std::string redirect_uri = "http://127.0.0.1:8080/";

    // Build request string
    std::string request =
        "code=" + auth_code +
        "&client_id=" + client_id +
        "&client_secret=" + client_secret +
        "&redirect_uri=" + redirect_uri +
        "&grant_type=authorization_code";

    // Build request to Google's oauth2 token endpoint
    curl_easy_setopt(curl, CURLOPT_URL, "https://oauth2.googleapis.com/token");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());

    // Specifications for capturing response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // HTTPS settings
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    // Execute request
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK);
}

// Callback function to collect response data 
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


