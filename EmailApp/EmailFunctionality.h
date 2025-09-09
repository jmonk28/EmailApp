#pragma once
#include "framework.h"
#include <string>

std::string getAuthCode();

void openBrowser(std::string& url);

std::string listenForAuthRedirect();

bool getAccessToken(std::string& response);

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)