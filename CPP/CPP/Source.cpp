#include <iostream>
#include "CBRS.h"
using namespace std;


int main(int argc, char** argv) 
{
	json j = R"(
        {
            "GAA": "http://127.0.0.1:8000/apigaa/1/",
            "PAL": "http://127.0.0.1:8000/apipal/3/",
            "price": "333.00",
            "orderDatetime": "2020-07-28T23:33:31.117213Z",
            "isFinish": false,
            "dealDateTime": null
        }
	)"_json;
	auto jp = CBRS::create_order(j);
	cout << jp.text << endl;
	return 0;
}