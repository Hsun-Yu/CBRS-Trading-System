#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <ctime>
using json = nlohmann::json;
using namespace std;

#pragma warning (disable : 4996) //_CRT_SECURE_NO_WARNINGS
#pragma once
namespace CBRS {
	const string SERVER_URL = "http://127.0.0.1:8000/";
	string now();

	json get(string url);
	json get_results(string url);
	json get_user(string url);
	json get_pal(string url);
	json get_gaa(string url);
	json get_order(string url);

	json all_object_to_url(json object);

	cpr::Response create_order(json order);
	cpr::Response create_pal_history(json pal_history);
	cpr::Response create_user(json user);
	cpr::Response create_pal(json pal);
	cpr::Response create_gaa(json gaa);

	cpr::Response update_order(string order_url, json order);
	cpr::Response update_pal(string order_url, json pal);
	cpr::Response deal_order(string order_url);

	string condition_json_to_string(json condition);
	json find_gaa(json condition);
	json find_pal(json condition);
	json find_order(json condition);
}