#include "CBRS.h"

string CBRS::now()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
	string str(buffer);
	return str;
}

json CBRS::get(string url)
{
	if (url.find("?format=json") == std::string::npos)
		url = url + "?format=json";
	cpr::Response r = cpr::Get(cpr::Url{ url });
	return json::parse(r.text);
}

json CBRS::get_results(string url)
{
	if (url.find("?format=json") == std::string::npos)
		url = url + "?format=json";
	cpr::Response r = cpr::Get(cpr::Url{ url });
	return json::parse(r.text)["results"];
}

json CBRS::get_user(string url)
{
	json re_json = CBRS::get(url);
	json groups;
	for (int i = 0; i < re_json["groups"].size(); i++) {
		string group_url = re_json["groups"][i];
		groups.push_back(CBRS::get(group_url));
	}
	re_json["groups"] = groups;
	return re_json;
}

json CBRS::get_pal(string url)
{
	json re_json = CBRS::get(url);
	string user_url = re_json["user"];
	re_json["user"] = CBRS::get_user(user_url);
	return re_json;
}

json CBRS::get_gaa(string url)
{
	json re_json = CBRS::get(url);
	string user_url = re_json["user"];
	string status_url = re_json["status"];

	re_json["user"] = CBRS::get_user(user_url);
	re_json["status"] = CBRS::get(status_url);

	if (!re_json["preference"].is_null())
	{
		string preference_url = re_json["preference"];
		re_json["preference"] = CBRS::get_pal(preference_url);
	}
	return re_json;
}

json CBRS::get_order(string url)
{
	json re_json = CBRS::get(url);
	string gaa_url = re_json["GAA"];
	string pal_url = re_json["PAL"];
	re_json["GAA"] = get_gaa(gaa_url);
	re_json["PAL"] = get_pal(pal_url);
	return re_json;
}

json CBRS::all_object_to_url(json object)
{
	for (json::iterator it = object.begin(); it != object.end(); it++)
	{
		if (it.value().is_object())
			it.value() = it.value()["url"];
	}
	return object;
}

cpr::Response CBRS::create_order(json order)
{
	json obj = CBRS::all_object_to_url(order);
	obj["orderDatetime"] = CBRS::now();
	cpr::Response resp = cpr::Post(cpr::Url{ CBRS::SERVER_URL + "apiorder/" },
		cpr::Body{ obj.dump() },
		cpr::Header{ {"Content-Type", "application/json"} });
	return resp;
}

cpr::Response CBRS::create_pal_history(json pal_history)
{
	json obj = CBRS::all_object_to_url(pal_history);

	cpr::Response resp = cpr::Post(cpr::Url{ CBRS::SERVER_URL + "apipalhistory/" },
		cpr::Body{ obj.dump() },
		cpr::Header{ {"Content-Type", "application/json"} });
	return resp;
}

cpr::Response CBRS::create_user(json user)
{
	json obj = CBRS::all_object_to_url(user);

	cpr::Response resp = cpr::Post(cpr::Url{ CBRS::SERVER_URL + "apiuser/" },
		cpr::Body{ obj.dump() },
		cpr::Header{ {"Content-Type", "application/json"} });
	return resp;
}

cpr::Response CBRS::create_pal(json pal)
{
	// Create PAL user
	if (pal["user"].is_object())
	{
		pal["user"]["groups"] = { CBRS::SERVER_URL + "apigroup/1/?format=json" };
		string user = CBRS::create_user(pal["user"]).text;
		pal["user"] = json::parse(user);
	}

	// Create PAL
	json obj = CBRS::all_object_to_url(pal);
	cpr::Response resp = cpr::Post(cpr::Url{ CBRS::SERVER_URL + "apipal/" },
		cpr::Body{ obj.dump() },
		cpr::Header{ {"Content-Type", "application/json"} });

	string new_pal_url = json::parse(resp.text)["url"];

	// Create PAL History
	string now = CBRS::now();
	json history = {
		{"PAL", new_pal_url},
		{"price" , pal["price"]},
		{"updateDateTime", now}
	};
	CBRS::create_pal_history(history);
	return resp;
}

cpr::Response CBRS::create_gaa(json gaa)
{
	// Create GAA user
	if (gaa["user"].is_object())
	{
		gaa["user"]["groups"] = { CBRS::SERVER_URL + "apigroup/2/?format=json" };
		string user = CBRS::create_user(gaa["user"]).text;
		gaa["user"] = json::parse(user);
	}

	// Create GAA
	json obj = CBRS::all_object_to_url(gaa);
	cpr::Response resp = cpr::Post(cpr::Url{ CBRS::SERVER_URL + "apigaa/" },
		cpr::Body{ obj.dump() },
		cpr::Header{ {"Content-Type", "application/json"} });

	return resp;
}

cpr::Response CBRS::update_order(string order_url, json order)
{
	json obj = CBRS::all_object_to_url(order);
	cpr::Response resp = cpr::Patch(cpr::Url{ order_url },
		cpr::Body{ obj.dump() },
		cpr::Header{ {"Content-Type", "application/json"} });

	return resp;
}

cpr::Response CBRS::update_pal(string pal_url, json pal)
{
	json obj = CBRS::all_object_to_url(pal);
	cpr::Response resp = cpr::Patch(cpr::Url{ pal_url },
		cpr::Body{ obj.dump() },
		cpr::Header{ {"Content-Type", "application/json"} });
	json history = {
		{"PAL", pal_url},
		{"price", pal["price"]}
	};
	cpr::Response re = create_pal_history(history);
	return resp;
}

cpr::Response CBRS::deal_order(string order_url)
{
	json order = {
		{"isFinish", true},
		{"dealDateTime", CBRS::now()}
	};

	cpr::Response resp = CBRS::update_order(order_url, order);
	return resp;
}

string CBRS::condition_json_to_string(json condition)
{
	string re = "?";
	for (json::iterator it = condition.begin(); it != condition.end(); it++)
	{
		re.append(it.key());
		re.append("=");
		re.append(it.value());
		re.append("&");
	}
	return re;
}

json CBRS::find_gaa(json condition)
{
	string condi_str = CBRS::condition_json_to_string(condition);
	string url = CBRS::SERVER_URL + "api/gaa/" + condi_str + "format=json";
	cpr::Response r = cpr::Get(cpr::Url{ url });
	return json::parse(r.text);
}

json CBRS::find_pal(json condition)
{
	string condi_str = CBRS::condition_json_to_string(condition);
	string url = CBRS::SERVER_URL + "api/pal/" + condi_str + "format=json";
	cpr::Response r = cpr::Get(cpr::Url{ url });
	return json::parse(r.text);
}

json CBRS::find_order(json condition)
{
	string condi_str = CBRS::condition_json_to_string(condition);
	string url = CBRS::SERVER_URL + "api/ods/" + condi_str + "format=json";
	cpr::Response r = cpr::Get(cpr::Url{ url });
	return json::parse(r.text);
}
