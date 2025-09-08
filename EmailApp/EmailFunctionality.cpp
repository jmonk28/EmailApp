#include "EmailFunctionality.h"
#include "Utilities.h"
#include "Resource.h"
#include "framework.h"
#include <curl/curl.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include <Windows.h>
#include <shellapi.h>

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

std::string listenForAuthRedirect() {

}


