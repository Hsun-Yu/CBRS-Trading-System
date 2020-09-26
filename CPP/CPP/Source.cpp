#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <fstream>
#include <random>
#include <iomanip>
#include <algorithm>
using namespace std;

#include "CBRS.h"
#include <string> 
//float apple ;

//unsigned remove(nlohmann::json& jsonObject, const std::string& value) {
//	std::vector<int> toremove;
//	//Loop through the `tags` json::array and create a vector of indexes to delete:
//	for (auto& it : jsonObject.items()) {
//		//`.get<std::string>()` is the best way to make sure we are getting the value as std::string
//		if (it.value().at("ROIId").get<std::string>() == value)
//			toremove.push_back(stoi(it.key()));
//	}
//	//sort it before erase - we want to delete first biggest index:
//	std::sort(toremove.rbegin(), toremove.rend());
//	//delete using `.erase()` method:
//	for (int& it : toremove)
//		jsonObject["Tags"].erase(jsonObject.begin() + it);
//	return toremove.size();
//}

string now()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%Y%m%dT%H%M%SZ", timeinfo);
	string str(buffer);
	return str;
}

int main()
{

	json pals = CBRS::find_pal(json());
	json gaas = json();
	json orders = json();

	float now_timing = 0;
	float end_time = 1.000; //0.051
	float i = 0.001;
	double sum = 0;
	int sum_c = 0;
	int normal_i;

	float delta = 0.002; //0.002

	int prference[] = { 1, 2, 3 };
	int willing[] = { 100, 200, 300, 400, 500 }; //{100, 120, 140, 160, 180}
	int a;
	int b;
	int channel_amount_of_mine = 10;
	int channel_amount_of_competitor = 10;

	pals[0]["numberOfRemaining"] = 10;
	pals[1]["numberOfRemaining"] = 10;

	//use for list
	std::list<float> Customer_timing_list;
	std::list<int> Preference_list;
	std::list<int> Willing_price_list;
	float my_timing;
	int my_preference;
	int my_willing;

	//Set the service time
	float service_time = 0.003;

	//Competitor's information
	int Competitor_price = 0;

	srand(time(NULL));

	//In change price function
	int Set_price = 0;

	//In determine function
	int Buy_price = 0;

	//For calculate revenue & selling amount
	int Total_my_revenue = 0;
	int Total_competitor_revenue = 0;
	int Number_my_selling = 0;
	int Number_competitor_selling = 0;

	//For calculate different preference's selling amount
	int Number_of_my_preference = 0;
	int Number_of_no_preference_buy_me = 0;
	int Number_of_competitor_buy_me = 0;

	int Number_of_my_preference_earn_by_competitor = 0;
	int Number_of_no_preference_earn_by_competitor = 0;
	int Number_of_competitor_earn_by_competitor = 0;

	//For calculate customer's price that I sold
	std::list<double> sum_price_I_sold_list;
	int sum_price_I_sold = 0;
	double average_price_I_sold = 0;
	double sum_square_price_I_sold = 0;
	double Standard_deviation_of_price_I_sold = 0;

	double lower_bound1 = 0;
	double upper_bound1 = 0;
	double upper_bound2 = 0;

	//old method of calculate lower & upper bound
	double lower_bound = 0;
	double upper_bound = 0;

	int count = 0;

	//For two-step selling
	double channel_amount_of_1st_step = channel_amount_of_mine * 0.8;
	double channel_amount_of_2nd_step = channel_amount_of_mine * 0.2;
	int lower_bound_array[500];
	int upper_bound_array[500];
	int upper_bound2_array[500];

	int rest_channel_of_1st_step = 0;
	int count_rest = 1; //for add rest 1st step channel amount

	//For calculate total customer
	int customer_num = 0;

	int select_pal = 0;
	int select_pal_price[] = { 100, 100 };

	int count_lower_bound = 1;
	int count_upper_bound = 1;

	//定義文件輸出流
	ofstream oFile;
	ofstream aFile;
	ofstream bFile;
	ofstream cFile;
	ofstream dFile;
	ofstream eFile;
	ofstream fFile;
	ofstream gFile;
	ofstream hFile;
	ofstream iFile;
	ofstream jFile;
	ofstream kFile;
	ofstream lFile;
	ofstream mFile;
	ofstream nFile;
	ofstream pFile;
	ofstream sFile;


	//打開要輸出的文件
	oFile.open("scoresheet.csv", ios::out | ios::trunc); // 這樣就很容易的输出一個需要的excel文件
	oFile << "買方來臨時間點"
		<< ","
		<< "偏好"
		<< ","
		<< "願付價格"
		<< ","
		<< "出售時間"
		<< ","
		<< "最終賣出價格" << endl;

	aFile.open("1.我方與對手售出時機點&價格.csv", ios::out | ios::trunc);
	aFile << "買方向我方購買時間"
		<< ","
		<< "買方願付價格"
		<< ","
		<< "買方購買價格"
		<< ","
		<< ","
		<< ","
		<< "買方向對手購買時間"
		<< ","
		<< "買方願付價格"
		<< ","
		<< "買方購買價格" << endl;

	bFile.open("2.統計未被滿足的客群.csv", ios::out | ios::trunc);
	bFile << "我方總賣出人數"
		<< ","
		<< "對手總賣出人數"
		<< ","
		<< "買方購買/來臨時間"
		<< ","
		<< "未被滿足的客群" << endl;

	cFile.open("3.統計被滿足的客群.csv", ios::out | ios::trunc);
	cFile << "售出時間"
		<< ","
		<< "買方最終向我方購買價格"
		<< ","
		<< "買方最終向競爭者購買價格"
		<< ","
		<< "願付價格"
		<< ","
		<< "偏好" << endl;

	dFile.open("4.統計偏好我方但未被滿足的客群.csv", ios::out | ios::trunc);
	dFile << "現在時間"
		<< ","
		<< "銷售狀態"
		<< ","
		<< "買方偏好"
		<< ","
		<< "買方願付價格" << endl;

	eFile.open("5.統計偏好對手但未被對手滿足的客群.csv", ios::out | ios::trunc);
	eFile << "現在時間"
		<< ","
		<< "銷售狀態"
		<< ","
		<< "買方偏好"
		<< ","
		<< "買方願付價格" << endl;

	fFile.open("6.傾向購買我方的實際跟誰購買、偏好、願付價格、實際購買價格.csv", ios::out | ios::trunc);
	fFile << "現在時間"
		<< ","
		<< "銷售狀態"
		<< ","
		<< "買方偏好"
		<< ","
		<< "買方願付價格"
		<< ","
		<< "實際購買價格" << endl;

	gFile.open("7.我方與對手售完時間.csv", ios::out | ios::trunc);
	gFile << "我方售完時間"
		<< ","
		<< "對手售完時間" << endl;

	hFile.open("8.我方與對手總體收入總和.csv", ios::out | ios::trunc);
	hFile << "我方與對手的收入總和" << endl;

	iFile.open("9.個別與整體總賣出頻寬數.csv", ios::out | ios::trunc);
	iFile << "我方所賣出的頻寬數"
		<< ","
		<< "對手所賣出的頻寬數"
		<< ","
		<< "整體所賣的頻寬數" << endl;

	jFile.open("10.使用者購買價格意願分佈.csv", ios::out | ios::trunc);
	jFile << "抵達時間"
		<< ","
		<< "願付價格"
		<< ","
		<< "偏好" << endl;

	kFile.open("11.連續來使得價格來不及更變的買方.csv", ios::out | ios::trunc);
	kFile << "買方抵達時間"
		<< ","
		<< "買方願付價格"
		<< ","
		<< "買方偏好" << endl;

	lFile.open("12.此次銷售所求得的平均售價與標準差.csv", ios::out | ios::trunc);
	lFile << "平均值"
		<< ","
		<< "標準差"
		<< ","
		<< "lower bound"
		<< ","
		<< "upper bound"
		<< ","
		<< "upper bound 2" << endl;

	mFile.open("13.plot for total revenue.csv", ios::out | ios::trunc);
	mFile << "Total my revenue"
		<< ","
		<< "Total competitor revenue" << endl;

	nFile.open("14.plot for willing price distribution.csv", ios::out | ios::trunc);
	nFile << "arrive time"
		<< ","
		<< "willing price"
		<< ","
		<< "preference" << endl;

	pFile.open("15.plot for total customer number.csv", ios::out | ios::trunc);
	pFile << "Total number" << endl;

	sFile.open("18.Revenue and sales volume.csv", ios::out | ios::trunc);
	sFile << "Our revenue"
		<< ","
		<< "Competitor Revenue"
		<< ","
		<< ","
		<< "Our sales volume"
		<< ","
		<< "Competitor sales volume"
		<< ","
		<< ","
		<< "We sold to our preference"
		<< ","
		<< "We sold to competitor's revenue"
		<< ","
		<< "Competitor sold to our preference"
		<< ","
		<< "Competitor sold to their preference" << endl;

	//int count = 0 ;
	while (count < 100)
	{
		/******************************************************產生gFile Excel檔**********************************************************/
		int n1 = 1;
		int n2 = 1;
		/******************************************************產生gFile Excel檔**********************************************************/

		lower_bound_array[count] = lower_bound1;
		lFile << ","
			<< ","
			<< ","
			<< ","
			<< "," << count + 2 << ".th" << endl;

		//Normal distribution for generating willing price
		default_random_engine generator;
		normal_distribution<double> distribution(5.0, 1.5);

		for (now_timing = 0.001; now_timing < end_time; now_timing = now_timing + i)
		{
			/********************For add remaining channel of 1st step to 2nd step********************/
			if ((now_timing > end_time / 2) && (channel_amount_of_1st_step != 0) && (count_rest == 1))
			{
				channel_amount_of_2nd_step += channel_amount_of_1st_step;
				channel_amount_of_1st_step = 0;
				printf("channel_amount_of_2nd_step : %lf\n", channel_amount_of_2nd_step);
				count_rest--;
			}
			/********************For add remaining channel of 1st step to 2nd step********************/

			/********************For original selling method if we going to can not sold out********************/
			if ((now_timing >= 0.9) && (channel_amount_of_2nd_step != 0))
			{
				upper_bound1 = 0;
				upper_bound2 = 0;
			}
			/********************For original selling method if we going to can not sold out********************/

			//printf("Test for now_timing : %.3f\n", now_timing);
			//printf("Test for Preference_list.front() : %d\n", Preference_list.front());
			//printf("Test for Customer_timing_list.front() : %.3f\n", Customer_timing_list.front());

			// IF the front of customer is out of service time
			// QUESTION: front() why ?
			// TODO: DELETE GAA API
			if ((Preference_list.size() > 0) && (int((now_timing * 1000) + 0.5) > int(((Customer_timing_list.front() + service_time) * 1000) + 0.5)))
			{
				Preference_list.pop_front();
				gaas.erase(gaas.begin());
			}
			// QUESTION: front() why ?
			// IF the front of customer is out of service time
			if ((Willing_price_list.size() > 0) && (int((now_timing * 1000) + 0.5) > int(((Customer_timing_list.front() + service_time) * 1000) + 0.5)))
			{
				Willing_price_list.pop_front();
			}
			// QUESTION: front() why ?
			// IF the front of customer is out of service time
			if ((Customer_timing_list.size() > 0) && (int((now_timing * 1000) + 0.5) > int(((Customer_timing_list.front() + service_time) * 1000) + 0.5)))
			{
				Customer_timing_list.pop_front();
			}


			// Generate diffrent probability of arrival
			while (sum_c < 3)
			{
				sum += (double)rand() / (RAND_MAX + 1.0);
				sum_c++; //會加到100
			}
			sum = sum / sum_c;
			printf("The time is : %.3f (s)\n", now_timing);
			// Customer come!!
			// QUESTION: is it using cumulative function?
			if (sum < 0.238)
			{ //當機率小於0.3時代表有一個買方到來
				printf("*******************Customer comes !!! and now time is : %.3f(s)*******************\n", now_timing);

				customer_num++; //For culculate customer number

				if ((now_timing + service_time) >= end_time)
				{
					printf("Sorry !!! It's too late to come, so we can service....\n");
				}

				a = (rand() % 3);
				printf("customer's peference : %d\n", prference[a]);

				b = (rand() % 5);
				//Normal distribution
				double number = distribution(generator);
				/*
				QUESTION: What is the E[X] and var(X)
				normal_distribution (5.0,2.0):
				0-1: *
				1-2: ****
				2-3: *********
				3-4: ***************
				4-5: ******************
				5-6: *******************
				6-7: ***************
				7-8: ********
				8-9: ****
				9-10: *
				*/
				// QUESTION: What is this? willing[b]?
				if ((number >= 0.0) && (number < 10.0))
				{
					if (int(number) == 4)
					{
						willing[b] = 100;
					}
					else if (int(number) == 6)
					{
						willing[b] = 200;
					}
					else if (int(number) == 5)
					{
						willing[b] = 300;
					}
					else if (int(number) == 7)
					{
						willing[b] = 400;
					}
					else if (int(number) == 8)
					{
						willing[b] = 500;
					}
				}

				printf("customer's willing price : %d\n", willing[b]);

				printf("***********************************************************************************\n");

				/******************************************************產生Excel檔**********************************************************/
				oFile << now_timing << "," << prference[a] << "," << willing[b] << endl; // For excel
				jFile << now_timing << "," << willing[b] << "," << prference[a] << endl; // For excel
				nFile << now_timing << "," << willing[b] << "," << prference[a] << endl; // For excel
				/******************************************************產生Excel檔**********************************************************/
				/***********************Store the customer's timing*********************/
				my_timing = now_timing;
				Customer_timing_list.push_back(my_timing);
				/***********************Store the customer's timing*********************/

				/***********************Store the customer's preference*********************/
				my_preference = prference[a];
				Preference_list.push_back(my_preference);
				/***********************Store the customer's preference*********************/

				/***********************Store the customer's willing price*********************/
				my_willing = willing[b];
				Willing_price_list.push_back(my_willing);
				/***********************Store the customer's willing price*********************/

				// TODO: Create or Update new GAA with my_preference and my_willing
				json GAA = {
					{
						"user", 
							{
								{"username", "GAA" + now()},
								{"password", "b10730229"}
							}
					}
				};
				string preurl = "";
				if (my_preference == 1)
					preurl = pals[0]["url"];
				else if (my_preference == 3)
					preurl = pals[1]["url"];
				if (preurl != "")
					GAA["preference"] = preurl;

				auto re = CBRS::create_gaa(GAA);
				gaas.push_back(json::parse(re.text));
				/******************************************************產生Excel檔**********************************************************/
				if ((Customer_timing_list.size() >= 2) && (Preference_list.size() >= 2) && (Willing_price_list.size() >= 2))
				{

					if ((now_timing - Customer_timing_list.front()) < 0.002)
					{

						for (std::list<float>::iterator a = Customer_timing_list.begin(); a != Customer_timing_list.end(); ++a)
							kFile << ' ' << *a;

						for (std::list<int>::iterator c = Willing_price_list.begin(); c != Willing_price_list.end(); ++c)
							kFile << "," << ' ' << *c;

						for (std::list<int>::iterator b = Preference_list.begin(); b != Preference_list.end(); ++b)
							kFile << ","
							<< "," << ' ' << *b;
						kFile << '\n';
					}
				}
				/******************************************************產生Excel檔**********************************************************/

				/******************************************************產生Excel檔**********************************************************/
				if (((channel_amount_of_1st_step + channel_amount_of_2nd_step) == 0) && (channel_amount_of_competitor == 0))
				{
					bFile << ","
						<< ","
						<< "現在時間(買方來的時間):" << now_timing << ","
						<< "雙方都已賣完但仍前來的客群(偏好/願付):"
						<< "," << prference[a] << "," << willing[b] << endl;

					if (prference[a] == 1)
					{
						dFile << "(抵達時間)" << now_timing << ","
							<< "買方未買到-雙方都已賣完但仍前來的客群"
							<< "," << prference[a] << "," << willing[b] << endl;
					}
					else if (prference[a] == 3)
					{
						eFile << "(抵達時間)" << now_timing << ","
							<< "買方未買到-雙方都已賣完但仍前來的客群"
							<< "," << prference[a] << "," << willing[b] << endl;
					}
				}
				/******************************************************產生Excel檔**********************************************************/
			}
			// End of Customer come!!
			else
			{
				//printf("no customer comes\n");
			}

			//float num;
			int tmp;
			int delta1;

			tmp = int((now_timing * 1000) + 0.5); //
			delta1 = int(delta * 1000);           //delta=0.002

			// IF (now timing in first step and the first step has remaining channel) OR (now timing in secend step and the secend step has remaining channel)
			if (((tmp % delta1 == 0) && (now_timing <= end_time / 2) && (channel_amount_of_1st_step != 0)) || ((tmp % delta1 == 0) && (now_timing > end_time / 2) && (channel_amount_of_2nd_step != 0)))
			{ //determine it is the time to change price or not
				printf("*******It's the time can change price ! Because now time %.3f is 0.002's multiple.********\n", now_timing);

				// Print out all informations
				std::cout << "Customer_timing_list contains:";
				for (std::list<float>::iterator it = Customer_timing_list.begin(); it != Customer_timing_list.end(); ++it)
					std::cout << ' ' << *it;
				std::cout << '\n';

				std::cout << "Preference_list contains:";
				for (std::list<int>::iterator it = Preference_list.begin(); it != Preference_list.end(); ++it)
					std::cout << ' ' << *it;
				std::cout << '\n';

				std::cout << "Willing_price_list contains:";
				for (std::list<int>::iterator it = Willing_price_list.begin(); it != Willing_price_list.end(); ++it)
					std::cout << ' ' << *it;
				std::cout << '\n';
				// =========================================

				/***********Function of P_1-i change price******************/
				if (channel_amount_of_competitor != 0)
				{
					Competitor_price = 100;
				}
				else
				{
					Competitor_price = 100000;
				}
				/***********************************************************/

				/*************************************************************Start change price*************************************************************/
				if (!Preference_list.empty())
				{
					printf("*****************Start to change our price******************\n");
					if (Preference_list.front() == 1)
					{
						Set_price = Willing_price_list.front();
						printf("So we set for 1 : $%d\n", Set_price);

						/*****************Check if meet the upper and lower bound or not*****************/
						// First step
						if (now_timing <= end_time / 2)
						{
							// Out of range
							if (Set_price < lower_bound1 || ((Set_price >= lower_bound1) && channel_amount_of_1st_step == 0))
							{
								printf("lower_bound1 : %lf and remaining 1st_step channel amount : %lf\n", lower_bound1, channel_amount_of_1st_step);
								Set_price = 100000;
							}
						}
						// Secend step
						else
						{
							// Out of range
							if (Set_price < upper_bound2 || ((Set_price >= upper_bound2) && channel_amount_of_2nd_step == 0))
							{
								printf("upper_bound2 : %lf and remaining 2nd_step channel amount : %lf\n", upper_bound2, channel_amount_of_2nd_step);
								Set_price = 100000;
							}
						}
						/*****************Check if meet the upper and lower bound or not*****************/
					}
					else if ((Preference_list.front() == 3) && (Competitor_price > Willing_price_list.front()))
					{
						Set_price = Willing_price_list.front();
						printf("So we set for 3 when we can win : $%d\n", Set_price);

						/*****************Check if meet the upper and lower bound or not*****************/
						if (now_timing < end_time / 2)
						{
							if (Set_price < lower_bound1 || ((Set_price >= lower_bound1) && channel_amount_of_1st_step == 0))
							{
								printf("lower_bound1 : %lf and remaining 1st_step channel amount : %lf\n", lower_bound1, channel_amount_of_1st_step);
								Set_price = 100000;
							}
						}
						else
						{
							if (Set_price < upper_bound1 || ((Set_price >= upper_bound1) && channel_amount_of_2nd_step == 0))
							{
								printf("upper_bound1 : %lf and remaining 2nd_step channel amount : %lf\n", upper_bound1, channel_amount_of_2nd_step);
								Set_price = 100000;
							}
						}
						/*****************Check if meet the upper and lower bound or not*****************/
					}
					else
					{ //1. no preference : we set lower than competitor    2. preference = 3 && Competitor_price <= willing price : we also set lower than competitor
					  // QUESTION: we also set lower than competitor? always >>> Set_price = Willing_price_list.front();
						if (Willing_price_list.size() != 0)
						{
							// QUESTION: all price >= 100 ?
							if ((Willing_price_list.front() >= 100) && (Preference_list.front() == 3))
							{
								Set_price = Willing_price_list.front();
								printf("So we set for 3 but we can't win : $%d\n", Set_price);
							}
							else if ((Willing_price_list.front() >= 100) && (Preference_list.front() == 2))
							{
								Set_price = Willing_price_list.front();
								printf("So we set for 2 but we can win (lower than competitor) : $%d\n", Set_price);

								/*****************Check if meet the upper and lower bound or not*****************/
								if (now_timing < end_time / 2)
								{
									if (Set_price < lower_bound1 || ((Set_price >= lower_bound1) && channel_amount_of_1st_step == 0))
									{
										printf("lower_bound1 : %lf and remaining 1st_step channel amount : %lf\n", lower_bound1, channel_amount_of_1st_step);
										Set_price = 100000;
									}
								}
								else
								{
									if (Set_price < upper_bound1 || ((Set_price >= upper_bound1) && channel_amount_of_2nd_step == 0))
									{
										printf("upper_bound1 : %lf and remaining 2nd_step channel amount : %lf\n", upper_bound1, channel_amount_of_2nd_step);
										Set_price = 100000;
									}
								}
								/*****************Check if meet the upper and lower bound or not*****************/
							}
							else
							{
								Set_price = Willing_price_list.front();
								printf("So we set for 2 but we can win (equals to willing price) : $%d\n", Set_price);

								/*****************Check if meet the upper and lower bound or not*****************/
								if (now_timing < end_time / 2)
								{
									if (Set_price < lower_bound1 || ((Set_price >= lower_bound1) && channel_amount_of_1st_step == 0))
									{
										printf("lower_bound1 : %lf and remaining 1st_step channel amount : %lf\n", lower_bound1, channel_amount_of_1st_step);
										Set_price = 100000;
									}
								}
								else
								{
									if (Set_price < upper_bound1 || ((Set_price >= upper_bound1) && channel_amount_of_2nd_step == 0))
									{
										printf("upper_bound1 : %lf and remaining 2nd_step channel amount : %lf\n", upper_bound1, channel_amount_of_2nd_step);
										Set_price = 100000;
									}
								}
								/*****************Check if meet the upper and lower bound or not*****************/
							}
						}
					}

					printf("*****************Finish changing our price*****************\n");
				}

				/*************************************************************End change price*************************************************************/
			}
			else if (((now_timing <= end_time / 2) && (channel_amount_of_1st_step == 0)) || ((now_timing > end_time / 2) && (channel_amount_of_2nd_step == 0)))
			{
				Set_price = 100000; //當為delta時間點,但該階段可銷售的頻寬數以賣完

				/***********Function of P_1-i change price******************/
				if (channel_amount_of_competitor != 0)
				{
					Competitor_price = 100;
				}
				else
				{
					Competitor_price = 100000;
				}
				/***********************************************************/
			}
			// QUESTION: (tmp % delta1 != 0)?
			else
			{
				if (!Preference_list.empty())
				{
					if ((now_timing <= end_time / 2) && (channel_amount_of_1st_step != 0) && (Set_price < lower_bound1))
					{
						Set_price = 100000; //當不為delta時間點,但因為買方連續來,導致會用到前一個買方的定價,所以要再判斷是否符合lower bound
					}
					else if ((now_timing > end_time / 2) && (channel_amount_of_2nd_step != 0))
					{ //當不為delta時間點,但因為買方連續來,導致會用到前一個買方的定價,所以要再判斷是否符合upper bound / upper bound 2

						if ((Preference_list.front() == 1) && (Set_price < upper_bound2))
						{
							Set_price = 100000;
						}
						else if (Set_price < upper_bound1)
						{
							Set_price = 100000;
						}
					}

					/***********Function of P_1-i change price******************/
					if (channel_amount_of_competitor != 0)
					{
						Competitor_price = 100;
					}
					else
					{
						Competitor_price = 100000;
					}
					/***********************************************************/
				}

			}
			// TODO: Edit PAL1 price
			// TODO: Edit PAL2 price
			select_pal_price[0] = Set_price;
			select_pal_price[1] = Competitor_price;
			if (Set_price)
			{
				pals[0]["price"] = Set_price;
				auto re1 = CBRS::update_pal(pals[0]["url"], pals[0]);
				cout << "Update PAL1 profile" << endl;
			}

			if (Competitor_price)
			{
				pals[1]["price"] = Competitor_price;
				auto re2 = CBRS::update_pal(pals[1]["url"], pals[1]);
				cout << "Update PAL2 profile" << endl;
			}

			/****************************************It's time to determine which to buy***************************************/

			if (!Customer_timing_list.empty())
			{
				// IF (now is in the service time) AND (I have remaining channel Or Competitor has remaining channel)
				if (((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (int((now_timing * 1000) + 0.5) == int(((Customer_timing_list.front() + service_time) * 1000) + 0.5)) || (channel_amount_of_competitor != 0) && (int((now_timing * 1000) + 0.5) == int(((Customer_timing_list.front() + service_time) * 1000) + 0.5)))
				{
					if (Preference_list.front() == 1)
					{
						if (Set_price <= Competitor_price)
						{
							if (Set_price <= Willing_price_list.front())
							{ //ex. Willing price : 140 , Me : 140 , Competitor : 140
							  // TODO: Create a order with PAL1 and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[0]["url"]},
									{"price", Set_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));

								Buy_price = Set_price;
								printf("Time %.3f Buyer is prefer 1 , and buy from me (my price <= willing price, competitor's price), and the price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << "," << Buy_price << ","
									<< "," << Willing_price_list.front() << ","
									<< "1" << endl; // For excel

								fFile << "(售出時間)" << now_timing << ","
									<< "買方買我方-我方定價<=對手定價"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << "," << Buy_price << endl;
								/****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else if (Set_price > Willing_price_list.front())
							{ //ex. Willing price : 140 , Me : 160 , Competitor : 180
								Buy_price = 0;
								printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < competitor's price < my price) when prefer 1.\n", now_timing);

								/******************************************************產生Excel檔**********************************************************/
								oFile << " "
									<< ","
									<< " "
									<< ","
									<< " "
									<< ","
									<< "售出時間 : " << now_timing << ","
									<< "***雙方定價皆高於偏好1買方的願付價格***" << endl; // For excel
								dFile << "(售出時間)" << now_timing << ","
									<< "買方未買到-我方&對手定價皆太高而失去的我方客群"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl;
								/******************************************************產生Excel檔**********************************************************/
							}
							else
							{
								printf("unknow 1 !!!\n");
							}
						}
						else if (Set_price == Competitor_price)
						{
							// QUESTION: Never happen?
							Buy_price = Set_price;
						}
						// My price is higher than competitor
						else
						{

							if (Set_price <= Willing_price_list.front())
							{ //ex. Willing price : 160 , Me : 160 , Competitor : 140
							  // TODO: Create a order with PAL1 and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[0]["url"]},
									{"price", Set_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));


								Buy_price = Set_price;
								printf("Time %.3f Buyer is prefer 1, and buy from me (competitor's price < my price <= willing price), and the price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << "," << Buy_price << ","
									<< "," << Willing_price_list.front() << ","
									<< "1" << endl; // For excel
								fFile << "(售出時間)" << now_timing << ","
									<< "買方買我方-我方定價比對手高"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << "," << Buy_price << endl;
								/****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else
							{
								if (Competitor_price <= Willing_price_list.front())
								{ //ex. Willing price : 140 , Me : 160 , Competitor : 140
								  // TODO: Create a order with PAL2 and the GAA

									json order = {
										{"GAA", gaas.front()["url"]},
										{"PAL", pals[1]["url"]},
										{"price", Competitor_price},
										{"isFinish", true},
										{"dealDateTime", CBRS::now()}
									};

									auto rr = CBRS::create_order(order);
									orders.push_back(json::parse(rr.text));

									Buy_price = Competitor_price;
									printf("Time %.3f Buyer is prefer 1, and buy from competitor (competitor's price <= willing price < my price), and the price is : %d\n", now_timing, Buy_price);

									/****************************************************************************************產生Excel檔***********************************************************************************/
									cFile << now_timing << ","
										<< "," << Buy_price << "," << Willing_price_list.front() << ","
										<< "1" << endl; // For excel
									fFile << "(售出時間)" << now_timing << ","
										<< "買方買對手-我方因為售完但對手滿足偏好我方的願付"
										<< "," << Preference_list.front() << "," << Willing_price_list.front() << "," << Buy_price << endl;
									/****************************************************************************************產生Excel檔***********************************************************************************/
								}
								else if (Competitor_price > Willing_price_list.front())
								{ //ex. Willing price : 120 , Me : 160 , Competitor : 140
									Buy_price = 0;
									printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < competitor's price < my price) when prefer 1.\n", now_timing);

									/******************************************************產生Excel檔**********************************************************/
									oFile << " "
										<< ","
										<< " "
										<< ","
										<< " "
										<< ","
										<< "售出時間 : " << now_timing << ","
										<< "***雙方定價皆高於偏好1買方的願付價格***" << endl; // For excel

									if (((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (channel_amount_of_competitor != 0))
									{
										dFile << "(售出時間)" << now_timing << ","
											<< "買方未買到-我方(較高)&對手定價皆太高而失去的我方客群"
											<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl;
									}
									/******************************************************產生Excel檔**********************************************************/
								}
								else
								{
									printf("unknow 2 !!!\n");
								}
							}
						}
					}
					else if (Preference_list.front() == 3)
					{
						if (Set_price <= Competitor_price)
						{
							if ((Set_price <= Willing_price_list.front()) && (Competitor_price > Willing_price_list.front()))
							{ //ex. Willing price : 120 , Me : 120 , Competitor : 140
							  // TODO: Create a order with PAL1 and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[0]["url"]},
									{"price", Set_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));
								Buy_price = Set_price;
								printf("Time %.3f Buyer is prefer 3, and buy from me the price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << "," << Buy_price << ","
									<< "," << Willing_price_list.front() << ","
									<< "3" << endl; // For excel
								eFile << "(賣出時間)" << now_timing << ","
									<< "買方買我方-買方偏好對手但對手定價高於買方願付"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl; // For excel
																													/****************************************************************************************產生Excel檔***********************************************************************************/
							}

							else if (Competitor_price <= Willing_price_list.front())
							{ //ex. Willing price : 140 , Me : 120 , Competitor : 140
							  // TODO: Create a order with PAL2 and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[1]["url"]},
									{"price", Competitor_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));
								Buy_price = Competitor_price;
								printf("Time %.3f Buyer is prefer 3, and buy from competition the price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << ","
									<< "," << Buy_price << "," << Willing_price_list.front() << ","
									<< "3" << endl; // For excel
							  /****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else if ((Competitor_price == Set_price) && (Competitor_price <= Willing_price_list.front()))
							{ //ex. Willing price : 160 , Me : 140 , Competitor : 140
							  // TODO: Create a order with PAL2 and the GAA

								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[1]["url"]},
									{"price", Competitor_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));

								Buy_price = Competitor_price;
								printf("Time %.3f Buyer is prefer 3, and buy from competition (my price = competitor's price <= willing price), and the price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << ","
									<< "," << Buy_price << "," << Willing_price_list.front() << ","
									<< "3" << endl; // For excel
							  /****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else if ((Competitor_price == Set_price) && (Set_price > Willing_price_list.front()))
							{ //ex. Willing price : 120 , Me : 140 , Competitor : 140
								Buy_price = 0;
								printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < competitor's price = my price) when prefer 3.\n", now_timing);

								/******************************************************產生Excel檔**********************************************************/
								oFile << " "
									<< ","
									<< " "
									<< ","
									<< " "
									<< ","
									<< "售出時間 : " << now_timing << ","
									<< "***雙方定價皆高於偏好3買方的願付價格***" << endl; // For excel
								eFile << "(售出時間)" << now_timing << ","
									<< "買方未買到-買方偏好對手但雙方定價皆高於買方願付"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl; // For excel
							  /******************************************************產生Excel檔**********************************************************/
							}
							else if ((Set_price > Willing_price_list.front()) && (Competitor_price > Willing_price_list.front()))
							{ //ex. Willing price : 100 , Me : 120 , Competitor : 140
								Buy_price = 0;
								printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < my price < competitor's price) when prefer 3.\n", now_timing);

								/******************************************************產生Excel檔**********************************************************/
								oFile << " "
									<< ","
									<< " "
									<< ","
									<< " "
									<< ","
									<< "售出時間 : " << now_timing << ","
									<< "***雙方定價皆高於偏好3買方的願付價格***" << endl; // For excel
								eFile << "(售出時間)" << now_timing << ","
									<< "買方未買到-買方偏好對手但雙方定價皆高於買方願付"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl; // For excel
							  /******************************************************產生Excel檔**********************************************************/
							}
							else
							{
								printf("unknow 3 !!!\n");
							}
						}
						else if (Set_price == Competitor_price)
						{
							// QUESTION: Never happen?
							Buy_price = Competitor_price;
						}
						else
						{
							if (Set_price <= Willing_price_list.front())
							{

								// ex. Willing price : 180 , Me : 160 , Competitor : 140
								// TODO: Create a order with PAL2 and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[1]["url"]},
									{"price", Competitor_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));
								Buy_price = Competitor_price;
								printf("Time %.3f Buyer is prefer 3, and buy from competitor (competitor's price < my price  <= willing price), and price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << ","
									<< "," << Buy_price << "," << Willing_price_list.front() << ","
									<< "3" << endl; // For excel
							  /****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else
							{
								if (Competitor_price <= Willing_price_list.front())
								{ //ex. Willing price : 140 , Me : sold out , Competitor : 140
								  // TODO: Create a order with PAL2 and the GAA

									json order = {
										{"GAA", gaas.front()["url"]},
										{"PAL", pals[1]["url"]},
										{"price", Competitor_price},
										{"isFinish", true},
										{"dealDateTime", CBRS::now()}
									};

									auto rr = CBRS::create_order(order);
									orders.push_back(json::parse(rr.text));

									Buy_price = Competitor_price;
									printf("Time %.3f Buyer is prefer 3, and buy from competitor (competitor's price <= willing price < my price), and price is : $%d \n", now_timing, Buy_price);

									/****************************************************************************************產生Excel檔***********************************************************************************/
									cFile << now_timing << ","
										<< "," << Buy_price << "," << Willing_price_list.front() << ","
										<< "3" << endl; // For excel
								  /****************************************************************************************產生Excel檔***********************************************************************************/
								}
								else if (Competitor_price > Willing_price_list.front())
								{ //ex. Willing price : 120 , Me : sold out , Competitor : 140
									Buy_price = 0;
									printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < competitor's price < my price) when prefer 3.\n", now_timing);

									/******************************************************產生Excel檔**********************************************************/
									oFile << " "
										<< ","
										<< " "
										<< ","
										<< " "
										<< ","
										<< "售出時間 : " << now_timing << ","
										<< "***雙方定價皆高於偏好3買方的願付價格***" << endl; // For excel
									eFile << "(售出時間)" << now_timing << ","
										<< "買方未買到-買方偏好對手但我方售完且對手定價高於買方願付"
										<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl; // For excel
								  /******************************************************產生Excel檔**********************************************************/
								}
								else
								{
									printf("unknow 4 !!!\n");
								}
							}
						}
					}

					else if (Preference_list.front() == 2)
					{

						if (Set_price < Competitor_price)
						{

							if (Set_price <= Willing_price_list.front())
							{ //ex. Willing price : 120 , Me : 120 , Competitor : 140
							  // TODO: Create a order with PAL1 and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[0]["url"]},
									{"price", Set_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));
								Buy_price = Set_price;
								printf("Time %.3f Buy from me (no preference and my price <= competitor's price), and price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << "," << Buy_price << ","
									<< "," << Willing_price_list.front() << ","
									<< "2" << endl; // For excel
							  /****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else if (Set_price > Willing_price_list.front())
							{ //ex. Willing price : 100 , Me : 120 , Competitor : 140
								Buy_price = 0;
								printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < my price < competitor's price) when no preference.\n", now_timing);

								/******************************************************產生Excel檔**********************************************************/
								oFile << " "
									<< ","
									<< " "
									<< ","
									<< " "
									<< ","
									<< "售出時間 : " << now_timing << ","
									<< "***雙方定價皆高於偏好2買方的願付價格***" << endl; // For excel
							  /******************************************************產生Excel檔**********************************************************/
							}
							else
							{
								printf("unknow 5 !!!\n");
							}
						}
						else if (Set_price == Competitor_price)
						{
							if ((Competitor_price <= Willing_price_list.front()) && (Set_price <= Willing_price_list.front()))
							{
								select_pal = (rand() % 2);
								printf("When two PAL set same price, we random select seller ! : %d\n", select_pal);
								Buy_price = select_pal_price[select_pal];
								printf("When two PAL set same price, no preference ! : $%d\n", select_pal_price[select_pal]);
								// TODO: Create a order with PAL1 or PAL2(select_pal) and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[select_pal]["url"]},
									{"price", Buy_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));
								/******************************************************產生Excel檔**********************************************************/
								if (select_pal == 0)
								{
									cFile << now_timing << "," << Buy_price << ","
										<< "," << Willing_price_list.front() << ","
										<< "2" << endl; // For excel
								}
								else if (select_pal == 1)
								{
									cFile << now_timing << ","
										<< "," << Buy_price << "," << Willing_price_list.front() << ","
										<< "2" << endl; // For excel
								}
								/******************************************************產生Excel檔**********************************************************/
							}
							else
							{
								Buy_price = 0;
								printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < my price < competitor's price) when no preference.\n", now_timing);
								/******************************************************產生Excel檔**********************************************************/
								oFile << " "
									<< ","
									<< " "
									<< ","
									<< " "
									<< ","
									<< "售出時間 : " << now_timing << ","
									<< "***雙方定價皆高於無偏好買方的願付價格***" << endl; // For excel
								eFile << "(售出時間)" << now_timing << ","
									<< "買方未買到-買方無偏好但雙方定價皆高於買方願付"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl; // For excel
							  /******************************************************產生Excel檔**********************************************************/
							}
						}
						else
						{

							if (Set_price <= Willing_price_list.front())
							{ //ex. Willing price : 180 , Me : 160 , Competitor : 140
							  // TODO: Create a order with PAL2 and the GAA
								Buy_price = Competitor_price;
								printf("Time %.3f Buyer is no preference, and buy from competitor (competitor's price < my price <= willing price), and price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << ","
									<< "," << Buy_price << "," << Willing_price_list.front() << ","
									<< "2" << endl; // For excel
							  /****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else if ((Set_price > Willing_price_list.front()) && (Competitor_price <= Willing_price_list.front()))
							{ //ex. Willing price : 140 , Me : 160 , Competitor : 140
							  // TODO: Create a order with PAL2 and the GAA
								json order = {
									{"GAA", gaas.front()["url"]},
									{"PAL", pals[1]["url"]},
									{"price", Competitor_price},
									{"isFinish", true},
									{"dealDateTime", CBRS::now()}
								};

								auto rr = CBRS::create_order(order);
								orders.push_back(json::parse(rr.text));
								Buy_price = Competitor_price;
								printf("Time %.3f Buyer is no preference, and buy from competitor (competitor's price <= willing price < my price), and price is : $%d \n", now_timing, Buy_price);

								/****************************************************************************************產生Excel檔***********************************************************************************/
								cFile << now_timing << ","
									<< "," << Buy_price << "," << Willing_price_list.front() << ","
									<< "2" << endl; // For excel
							  /****************************************************************************************產生Excel檔***********************************************************************************/
							}
							else if ((Set_price > Willing_price_list.front()) && (Competitor_price > Willing_price_list.front()))
							{ //ex. Willing price : 120 , Me : 160 , Competitor : 140
								Buy_price = 0;
								printf("Time %.3f Two of PALs price are higher than willing price !!! (willing price < competitor's price < my price ) when no preference.\n", now_timing);

								/******************************************************產生Excel檔**********************************************************/
								oFile << " "
									<< ","
									<< " "
									<< ","
									<< " "
									<< ","
									<< "售出時間 : " << now_timing << ","
									<< "***雙方定價皆高於偏好2買方的願付價格***" << endl; // For excel
							  /******************************************************產生Excel檔**********************************************************/
							}
							else
							{
								printf("unknow 6 !!!\n");
							}
						}
					}

					/****************************************It's time to determine which to buy***************************************/

					/****************************************Calculate the revenue*************************************/
					// Buy from me
					if (((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (Buy_price == Set_price) && (Buy_price != 0) && (Set_price != 0) && (Preference_list.front() == 1) ||
						((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (channel_amount_of_competitor == 0) && (Buy_price == Set_price) && (Buy_price != 0) && (Set_price != 0) && (Preference_list.front() == 3) ||
						((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (channel_amount_of_competitor != 0) && (Buy_price == Set_price) && (Buy_price != 0) && (Set_price != 0) && (Preference_list.front() == 2) && (select_pal == 0) ||
						((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (channel_amount_of_competitor == 0) && (Buy_price == Set_price) && (Buy_price != 0) && (Set_price != 0) && (Preference_list.front() == 2) ||
						((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (Buy_price == Set_price) && (Buy_price != 0) && (Set_price != 0) && (Set_price != Competitor_price))
					{
						printf("!!!!!!!!!!!!!!!!!!!Buy_price = %d!!!!!!!!!!!!!!!!!!!!\n", Buy_price);
						printf("!!!!!!!!!!!!!!!!!!!Set_price = %d!!!!!!!!!!!!!!!!!!!!\n", Set_price);

						// TODO: Create a order with PAL1 and the GAA
						// TODO: Edit PAL1 remaining
						Total_my_revenue += Buy_price;
						Number_my_selling++; // When Buy_price==Set_price, can not equals to 0

						if (now_timing <= end_time / 2)
						{
							channel_amount_of_1st_step--;
						}
						else if (now_timing > end_time / 2)
						{
							channel_amount_of_2nd_step--;
						}

						//channel_amount_of_mine -- ; //

						/******************************************************產生Excel檔**********************************************************/
						oFile << " "
							<< ","
							<< " "
							<< ","
							<< " "
							<< ","
							<< "售出時間 : " << now_timing << ","
							<< "買方最終向我方購買價格 : " << Buy_price << endl; // For excel
					  /******************************************************產生Excel檔**********************************************************/

					  /******************************************************產生Excel檔**********************************************************/
						aFile << now_timing << "," << Willing_price_list.front() << "," << Buy_price << endl; // For excel
						/******************************************************產生Excel檔**********************************************************/

						//For calcualte the lower bound
						sum_price_I_sold_list.push_back(Buy_price);

						if (Preference_list.front() == 1)
						{
							Number_of_my_preference++;
						}
						else if (Preference_list.front() == 2)
						{
							Number_of_no_preference_buy_me++;
						}
						else if (Preference_list.front() == 3)
						{
							Number_of_competitor_buy_me++;
						}
					}
					// Buy from competitor
					else if ((channel_amount_of_competitor != 0) && (Buy_price == Competitor_price) && (Buy_price != 0) && (Competitor_price != 0) && (Preference_list.front() == 3) ||
						(channel_amount_of_competitor != 0) && ((channel_amount_of_1st_step + channel_amount_of_2nd_step) == 0) && (Buy_price == Competitor_price) && (Buy_price != 0) && (Competitor_price != 0) && (Preference_list.front() == 1) ||
						(channel_amount_of_competitor != 0) && ((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0) && (Buy_price == Competitor_price) && (Buy_price != 0) && (Competitor_price != 0) && (Preference_list.front() == 2) && (select_pal == 1) ||
						(channel_amount_of_competitor != 0) && ((channel_amount_of_1st_step + channel_amount_of_2nd_step) == 0) && (Buy_price != 0) && (Competitor_price != 0) && (Preference_list.front() == 2) ||
						(channel_amount_of_competitor != 0) && (Buy_price == Competitor_price) && (Buy_price != 0) && (Competitor_price != 0) && (Competitor_price != Set_price))
					{
						// TODO: Create a order with PAL2 and the GAA
						// TODO: Edit PAL2 remaining
						Total_competitor_revenue += Buy_price;
						Number_competitor_selling++; // When Buy_price==Competitor_price, can not equals to 0

						channel_amount_of_competitor--;

						/******************************************************產生Excel檔**********************************************************/
						oFile << " "
							<< ","
							<< " "
							<< ","
							<< " "
							<< ","
							<< "售出時間 : " << now_timing << ","
							<< "買方最終向競爭者購買價格 : " << Buy_price << endl; // For excel
					  /******************************************************產生Excel檔**********************************************************/

					  /******************************************************產生Excel檔**********************************************************/
						aFile << ","
							<< ","
							<< ","
							<< ","
							<< "," << now_timing << "," << Willing_price_list.front() << "," << Buy_price << endl; // For excel
					  /******************************************************產生Excel檔**********************************************************/

					  /******************************************************產生Excel檔**********************************************************/
						if ((Preference_list.front() == 1))
						{
							bFile << ","
								<< ","
								<< "現在時間:" << now_timing << ","
								<< "我方售完,但買方偏好我而買對手的(偏好/願付):"
								<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl;
							dFile << "(售出時間)" << now_timing << ","
								<< "買方買對手-我方因為售完但對手滿足偏好我方的願付"
								<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl;
						}
						/******************************************************產生Excel檔**********************************************************/

						if (Preference_list.front() == 1)
						{
							Number_of_my_preference_earn_by_competitor++;
						}
						else if (Preference_list.front() == 2)
						{
							Number_of_no_preference_earn_by_competitor++;
						}
						else if (Preference_list.front() == 3)
						{
							Number_of_competitor_earn_by_competitor++;
						}
					}

					/******************************************************產生Excel檔**********************************************************/
					else if ((channel_amount_of_1st_step == 0) && (Buy_price != Set_price) || (channel_amount_of_2nd_step == 0) && (Buy_price != Set_price)) //我方售完,但對手還沒
					{
						if ((channel_amount_of_competitor != 0) && (Buy_price != Competitor_price))
						{
							bFile << ","
								<< ","
								<< "現在時間:" << now_timing << ","
								<< "我方因為賣完/對手定價太高而失去的客群(偏好/願付):"
								<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl;

							if (Preference_list.front() == 1)
							{
								dFile << "(售出時間)" << now_timing << ","
									<< "買方未買到-我方因為賣完&對手定價太高而失去的我方客群"
									<< "," << Preference_list.front() << "," << Willing_price_list.front() << endl;
							}
						}
					}
					/******************************************************產生Excel檔**********************************************************/
				}
			}
			/****************************************Calculate the revenue*************************************/

			/********************************************Check the channel is sold out or not*********************************************/
			if (((channel_amount_of_1st_step + channel_amount_of_2nd_step) == 0) && (channel_amount_of_competitor != 0))
			{
				printf("We already sold out our channel !!!!!!!!!!!!!!!\n");
				Set_price = 100000;

				/******************************************************產生Excel檔**********************************************************/
				if (n1 == 1)
				{
					gFile << now_timing << endl; // For excel
					n1--;
				}
				/******************************************************產生Excel檔**********************************************************/
			}
			else if ((channel_amount_of_competitor == 0) && ((channel_amount_of_1st_step + channel_amount_of_2nd_step) != 0))
			{
				printf("Competitor already sold out its channel !!!!!!!!!!!!!!!\n");
				Competitor_price = 100000;
			}
			else if ((channel_amount_of_1st_step + channel_amount_of_2nd_step == 0) && (channel_amount_of_competitor == 0))
			{
				printf("Two of PALs have no more channel can sell !!!!!!!!!!!!!!\n");
				Set_price = 100000;
				Competitor_price = 100000;

				/******************************************************產生Excel檔**********************************************************/
				if (n2 == 1)
				{
					gFile << "," << now_timing << endl; // For excel
					n2--;
				}
			}
			if ((now_timing >= 0.3) && (now_timing < 0.5) && (channel_amount_of_1st_step > 4) && (count_lower_bound == 1))
			{
				lower_bound1 = lower_bound1 - 100;
				count_lower_bound--;
				//printf("After lower_bound1 changed : %lf\n", lower_bound1) ;
			}
			if ((now_timing >= 0.75) && (now_timing < 0.9) && (channel_amount_of_2nd_step > 1) && (count_upper_bound == 1))
			{
				upper_bound1 = upper_bound1 - 100;
				upper_bound2 = upper_bound2 - 100;
				count_upper_bound--;
				//printf("After upper_bound1 changed : %lf\n", upper_bound1) ;
				//printf("After upper_bound2 changed : %lf\n", lower_bound1) ;
			}

			/********************************************Check the channel is sold out or not*********************************************/

			/*************************************Result****************************************/

			if (int((now_timing * 1000) + 0.5) == int((end_time * 1000) + 0.5))
			{
				printf("\n");
				printf("\n");
				printf("**************************************\n");
				printf("Total my revenue : %d\n", Total_my_revenue);
				printf("Total competitor revenue : %d\n", Total_competitor_revenue);
				printf("Total customer I selling : %d\n", Number_my_selling);
				printf("Total customer competitor selling : %d\n", Number_competitor_selling);
				printf("**************************************\n");

				/******************************************************產生Excel檔**********************************************************/
				bFile << Number_my_selling << "," << Number_competitor_selling << endl; // For excel
				/******************************************************產生Excel檔**********************************************************/

				/******************************************************產生Excel檔**********************************************************/
				cFile << "我方總銷售頻寬數 : " << Number_my_selling << ","
					<< "競爭者總銷售頻寬數 : " << Number_competitor_selling << endl; // For excel
			  /******************************************************產生Excel檔**********************************************************/

			  /******************************************************產生Excel檔**********************************************************/
				hFile << Total_my_revenue + Total_competitor_revenue << endl; // For excel
				/******************************************************產生Excel檔**********************************************************/

				/******************************************************產生Excel檔**********************************************************/
				iFile << Number_my_selling << "," << Number_competitor_selling << "," << Number_my_selling + Number_competitor_selling << endl; // For excel
				/******************************************************產生Excel檔**********************************************************/

				/******************************************************產生Excel檔**********************************************************/
				pFile << customer_num << endl; // For excel
				/******************************************************產生Excel檔**********************************************************/

				printf("\n");
				printf("********************************************************\n");
				printf("Total customer that prefer me & buy from me : %d\n", Number_of_my_preference);
				printf("Total customer that have no preference & buy from me : %d\n", Number_of_no_preference_buy_me);
				printf("Total customer that prefer competitor & buy from me : %d\n", Number_of_competitor_buy_me);
				printf("********************************************************\n");

				printf("\n");
				printf("****************************************************************\n");
				printf("Total customer that prefer me but buy from competitor : %d\n", Number_of_my_preference_earn_by_competitor);
				printf("Total customer that have no preference & buy from competitor : %d\n", Number_of_no_preference_earn_by_competitor);
				printf("Total customer that prefer competitor & buy from competitor : %d\n", Number_of_competitor_earn_by_competitor);
				printf("****************************************************************\n");

				printf("\n");
				printf("****************************************************************\n");
				printf("!!!!! Calculate the threshold for next selling (Round : %d) !!!!!\n", count + 1);

				cout << "The price of customers (I sold) : ";
				for (std::list<double>::iterator it = sum_price_I_sold_list.begin(); it != sum_price_I_sold_list.end(); ++it)
					cout << ' ' << *it;
				cout << '\n';

				cout << "The average of customers price (I sold) : ";
				for (std::list<double>::iterator a = sum_price_I_sold_list.begin(); a != sum_price_I_sold_list.end(); ++a)
					sum_price_I_sold += *a;
				average_price_I_sold = double(sum_price_I_sold) / double(10); //////////////
				cout << setprecision(4) << average_price_I_sold;
				cout << '\n';
				cout << "sum_price_I_sold : " << sum_price_I_sold << endl;
				cout << "Number_my_selling : " << Number_my_selling << endl;

				cout << "The squares of customers' price (I sold) : ";
				for (std::list<double>::iterator s = sum_price_I_sold_list.begin(); s != sum_price_I_sold_list.end(); ++s)
					cout << ' ' << (*s - average_price_I_sold) * (*s - average_price_I_sold);
				cout << '\n';

				for (std::list<double>::iterator s = sum_price_I_sold_list.begin(); s != sum_price_I_sold_list.end(); ++s)
					sum_square_price_I_sold += (*s - average_price_I_sold) * (*s - average_price_I_sold);
				cout << "Sum of squares : " << sum_square_price_I_sold;
				cout << '\n';

				Standard_deviation_of_price_I_sold = sqrt(sum_square_price_I_sold / 10);
				cout << "Standard deviation of price I sold : " << Standard_deviation_of_price_I_sold << endl;

				cout << "Lower bound which is set by prior selling : " << lower_bound1 << endl;
				cout << "Upper bound which is set by prior selling : " << upper_bound1 << endl;
				cout << "Upper bound 2 which is set by prior selling : " << upper_bound2 << endl;
				printf("Prior lower bound : %d\n", lower_bound_array[count - 1]);
				printf("****************************************************************\n");

				printf("****************************************************************\n");
				cout << "Old lower bound & upper bound !!!" << endl;
				cout << "lower bound : " << lower_bound << endl;
				cout << "upper bound : " << upper_bound << endl;
				printf("****************************************************************\n");

				printf("\n");

				/**********************************************產生Excel檔**************************************************/
				//lFile << average_price_I_sold << "," << Standard_deviation_of_price_I_sold << endl ;   //For excel
				//lFile << "," << "," << lower_bound1 << "," << upper_bound1 << "," << upper_bound2 << endl ;   //For excel
				/**********************************************產生Excel檔**************************************************/

				/******************************************************產生Excel檔**********************************************************/
				oFile << "我方總收入: $ " << Total_my_revenue << ","
					<< "偏好於我方的人數: " << Number_of_my_preference << ","
					<< "沒有偏好但買我方的人數: " << Number_of_no_preference_buy_me << ","
					<< "偏好於競爭者但買我方的人數: " << Number_of_competitor_buy_me << ","
					<< " " << endl; // For excel
				oFile << "競爭者總收入: $ " << Total_competitor_revenue << ","
					<< "偏好於我方但買競爭者的人數: " << Number_of_my_preference_earn_by_competitor << ","
					<< "沒有偏好但買競爭者的人數: " << Number_of_no_preference_earn_by_competitor << ","
					<< "偏好於競爭者的人數: " << Number_of_competitor_earn_by_competitor << ","
					<< " " << endl; // For excel
				sFile << Total_my_revenue << "," << Total_competitor_revenue << ","
					<< "," << Number_my_selling << "," << Number_competitor_selling << ","
					<< "," << Number_of_my_preference << "," << Number_of_competitor_buy_me << "," << Number_of_my_preference_earn_by_competitor << "," << Number_of_competitor_earn_by_competitor << endl;
				aFile << count + 1 << ".th"
					<< "我方總收入: $ " << Total_my_revenue << endl; // For excel
				mFile << Total_my_revenue << "," << Total_competitor_revenue << endl;
				/******************************************************產生Excel檔**********************************************************/

				/*************************************Result****************************************/

				lower_bound1 = int((average_price_I_sold - 1.96 * (Standard_deviation_of_price_I_sold / double(sqrt(10)))) + 0.5);
				upper_bound1 = int((average_price_I_sold + 1.96 * (Standard_deviation_of_price_I_sold / double(sqrt(10)))) + 0.5);
				upper_bound2 = int((average_price_I_sold + 2.575 * (Standard_deviation_of_price_I_sold / double(sqrt(10)))) + 0.5);

				/**********************************************產生Excel檔**************************************************/
				lFile << average_price_I_sold << "," << Standard_deviation_of_price_I_sold << endl; //For excel
				lFile << ","
					<< "," << lower_bound1 << "," << upper_bound1 << "," << upper_bound2 << endl; //For excel
			  /**********************************************產生Excel檔**************************************************/
				if (upper_bound1 > 500)
				{
					upper_bound1 = 500;
				}
				if (upper_bound2 > 500)
				{
					upper_bound2 = 500;
				}

				/*
		if( (now_timing >= 0.3) && (now_timing < 0.5) && (channel_amount_of_1st_step > 8)){
		  lower_bound1 = lower_bound1 - 100 ;
		  printf("After lower_bound1 changed : %lf\n", lower_bound1) ;
		}
		if((now_timing >= 0.75) && (now_timing < 0.9) && (channel_amount_of_2nd_step > 2)){
		  upper_bound1 = upper_bound1 - 100 ;
		  upper_bound2 = upper_bound2 - 100 ;
		  printf("After upper_bound1 changed : %lf\n", upper_bound1) ;
		  printf("After upper_bound2 changed : %lf\n", lower_bound1) ;
		}
		*/
		//if(count>=1){
		//lower_bound1 = int((lower_bound1/100.0)+0.5)*100 ;
		//upper_bound1 = int((upper_bound1/100.0)+0.5)*100 ;
		//upper_bound2 = int((upper_bound2/100.0)+0.5)*100 ;
		//}

		/**********************************************產生Excel檔**************************************************/
				lFile << ","
					<< ","
					<< ","
					<< ","
					<< "," << lower_bound1 << "," << upper_bound1 << "," << upper_bound2 << endl; //For excel
			  /**********************************************產生Excel檔**************************************************/
			}

			sum = 0;
			sum_c = 0;

		} // for loop end

		//此處為跑下一次for迴圈時須初始化的參數

		//Empty all list
		Customer_timing_list.clear();
		Preference_list.clear();
		Willing_price_list.clear();

		//Reallocate channel amount
		channel_amount_of_mine = 10;
		channel_amount_of_competitor = 10;

		//Initial the price
		Set_price = 0;
		Buy_price = 0;

		//Initial revenue & selling amount
		Total_my_revenue = 0;
		Total_competitor_revenue = 0;
		Number_my_selling = 0;
		Number_competitor_selling = 0;

		//Initial different preference's selling amount
		Number_of_my_preference = 0;
		Number_of_no_preference_buy_me = 0;
		Number_of_competitor_buy_me = 0;

		Number_of_my_preference_earn_by_competitor = 0;
		Number_of_no_preference_earn_by_competitor = 0;
		Number_of_competitor_earn_by_competitor = 0;

		//Initial customers' price that I sold
		sum_price_I_sold = 0;
		sum_price_I_sold_list.clear();
		sum_square_price_I_sold = 0;

		lower_bound = average_price_I_sold - 0.3 * Standard_deviation_of_price_I_sold;
		upper_bound = average_price_I_sold + 1 * Standard_deviation_of_price_I_sold;
		//lower_bound1 = average_price_I_sold - 1.96*(Standard_deviation_of_price_I_sold / double(sqrt(Number_my_selling)) ) ;
		//upper_bound1 = average_price_I_sold + 1.96*(Standard_deviation_of_price_I_sold / double(sqrt(Number_my_selling)) ) ;

		//For two-step selling
		channel_amount_of_1st_step = channel_amount_of_mine * 0.8;
		channel_amount_of_2nd_step = channel_amount_of_mine * 0.2;

		//
		rest_channel_of_1st_step = 0;
		count_rest = 1; //for add rest 1st step channel amount

		customer_num = 0;

		select_pal_price[0] = 100;
		select_pal_price[1] = 100;

		count_lower_bound = 1;
		count_upper_bound = 1;

		/******************************************************產生Excel檔**********************************************************/
		oFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		oFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		oFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		aFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		aFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		aFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		bFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		bFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		bFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		cFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		cFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		cFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		dFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		dFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		dFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		eFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		eFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		eFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		fFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		fFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		fFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		gFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		gFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		gFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		hFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		hFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		hFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		iFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		iFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		iFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		jFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		jFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		jFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		kFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		kFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		kFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  /******************************************************產生Excel檔**********************************************************/
		lFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excel
		lFile << "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********"
			<< ","
			<< "***********" << endl; // For excel
		lFile << " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " "
			<< ","
			<< " " << endl; // For excels
	  /******************************************************產生Excel檔**********************************************************/

	  //For while loop (做幾次for loop)
		count = count + 1;
	}

	/*******************************************產生Excel檔*******************************************/
	oFile.close();
	//ifstream iFile("scoresheet.csv");

	aFile.close();
	bFile.close();
	cFile.close();
	eFile.close();
	fFile.close();
	gFile.close();
	hFile.close();
	iFile.close();
	jFile.close();
	kFile.close();
	lFile.close();
	mFile.close();
	nFile.close();
	sFile.close();

	//string   readStr((std::istreambuf_iterator<char>(iFile)),  std::istreambuf_iterator<char>());
	//cout <<  readStr.c_str();
	/*******************************************產生Excel檔*******************************************/

	return 0;
}
