// VigenerCipher3.cpp : Defines the entry point for the console application.
//

// слово ключ 4 букви - EASY 
// за допомогою Квадрата Виженера і ключа знаходжу висхідний текст
//ADDTHEABILITYTODECIPHERANYKINDOFPOLYALPHABETICCIPHERSTHEONEUSEDINTHECIPHERTEXTSHEREHASTWENTYSIXINDEPENDENTRANDOMLYCHOSENMONOALPHABETICSUBSTITUTIONPATTERNSFOREACHLETTERFROMENGLISHALPHABETITISCLEARTHATYOUCANNOMORERELYONTHESAMESIMPLEROUTINEOFGUESSINGTHE

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <list>
#include <map>

using namespace std;

vector<int> getDistances(vector<int> dist, string cipher);
vector<int> getFactors(vector<int> factors, vector<int> distances);
int getKeyLength(set<int> lengths, vector<int> factors, string cipher);
float getIC(int len, string cipher);
void getAllX2(string cipher, int length);
float getX2(vector<char> Gx);

vector<int> distances;
vector<string> substrs;
vector<int> factors;
vector<pair<int, int>> keyLengths;
vector<pair<float, int>> res;
set<int> lengths;
typedef map<char, float > list_x2;

char engAlphab[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',	'H', 'I', 'J', 'K',	'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',	'X', 'Y', 'Z' };
float F_engFreqVal[] = { 0.082, 0.014, 0.028, 0.038, 0.131, 0.029, 0.020, 0.053, 0.064, 0.001, 0.004, 0.034, 0.025, 0.071, 0.080, 0.020, 0.001, 0.068, 0.061, 0.105, 0.025, 0.009, 0.015, 0.002, 0.020, 0.001 };


int main()
{
	//examples
	//string cipher = "LFWKIMJLFWLOOMULOOMKLOOLILOO";
	//string cipher = "LFWKIMJCLPSISWKHJOGLKMVGURAGKMKMXMAMJCVXWUYLGGIISWALXAEYCXMFKMKBQBDCLAEFLFWKIMJCGUZUGSKECZGBWYMOACFVMQKYFWXTWMLAIDOYQBWFGKSDIULQGVSYHJAVEFWBLAEFLFWKIMJCFHSNNGGNWPWDAVMQFAAXWFZCXBVELKWMLAVGKYEDEMJXHUXDAVYXL";//string cipher = "LFWKIMJCLPSISWKHJOGLKMVGURAGKMKMXMAMJCVXWUYLGGIISWALXAEYCXMFKMKBQBDCLAEFLFWKIMJCGUZUGSKECZGBWYMOACFVMQKYFWXTWMLAIDOYQBWFGKSDIULQGVSYHJAVEFWBLAEFLFWKIMJCFHSNNGGNWPWDAVMQFAAXWFZCXBVELKWMLAVGKYEDEMJXHUXDAVYXL";
	string cipher = "EDVRLESZMLARCTGBICANLEJYRYCGRDGDTODWELHFEBWRMCUGTHWPWTZCSNWSWEVGRTZCGIHFIRLCBTKFIRWFESLUINLWWIPGRDWNINVCRTJYRDGKPYUFSSWLQOFMELHFEBWRMCKSFSLGXULGSNHYXTWPRSXMVESALLWRXEJDVOECRGDGWHSJTHSZITARMSUJIAJRLALWSUUYRNGKSRWPILQMRTZCWAECWIENPEJMYTALIOXEYEKQMNYRLECCCBQCBHSSWTATISWYVCZULIUFCOMNVOTYFLQSWEVRSDWAMPZCVTZGWPSPEGJYTHOGPLLFIIFBIXGDGOALGIVCRCWQXIDJAOJIESSQYGYCWTAMRYGSGAFRVYLMHINGHELFIMWQWAYCMNHYVTKZCTZCRUEZIRGDGHSPECLCVSALEKWWENVYTPDWJRWOYEFACAFYPYKGWTGCECZMJTZCQCSLCOMDMNVYAAQRSUKCLIYFIRGPHEJDVEISINUWWTSRMSLGGSOGXHLFMSLWTEGDGIHFIRLFINWVXPSPEGJYTHUMRTSGRSKMQEHPITLWMNLCVEKRMNYGRFGPQALGSNSZSULQSMWYHDARMOFYPRWUERV";
	//string cipher = "VVQGYTVVVKALURWFHQACMMVLEHUCATWFHHIPLXHVUWSCIGINCMUHNHQRMSUIMHWZODXTNAEKVVQGYTVVQPHXINWCABASYYMTKSZRCXWRPRFWYHXYGFIPSBWKQAMZYBXJQQABJEMTCHQSNAEKVVQGYTVVPCAQPBSLURQUCVMVPQUTMMLVHWDHNFIKJCPXMYEIOCDTXBJWKQGAN";
	//string cipher = "RSTCSJLSLRSLFELGWLFIISIKRMGL";
	//string cipher = "WQXYMREOBPVWHTHQYEQVEDEXRBGSIZSILGRTAJFZOAMAVVXGRFQGKCPIOZIJBCBLUWYRWSTUGVQPSUDIUWOESFMTBTANCYZTKTYBVFDKDERSIBJECAQDWPDERIEKGPRAQFBGTHQKVVGRAXAVTHARQEELUECGVVBJEBXIJAKNGESWTKBEDXPBQOUDWVTXESMRUWWRPAWKMTITKHFWTDAURRVFESFESTKSHFLZAEONEXZBWTIARWWTTHQYEQVEDEXRBGSOREDMTICM";

	//getDistances between equal chars
	distances.swap(getDistances(distances, cipher));
	//get factors of distances
	//factors will help to get lengths of the keyword
	factors.swap(getFactors(factors, distances));
	//get length from lengths
	int length = getKeyLength(lengths, factors, cipher);
	//get X2 for getting letters of the keyword
	getAllX2(cipher, length);

	return 0;
}

//методом Касіскі
vector<int> getDistances(vector<int> dist, string cipher)
{

	//for (int i = 3; i < 8; i++) {
	//шукаю відстані між триграмами. якщо розкоментувати
	//строку вище, то можна шукати відстані від три до восьми грам
	for (int i = 0; i < cipher.size() - 2; i++)
	{
		//виділяю підстроку(триграму) для пошуку в шифрі
		string substring = cipher.substr(i, 3);
		//лічильник повторювань підстроки
		int count_coincid_sub = 0;
		//масив для збереження індексів триграм(їх початкової позиції)
		int sub_indexes[3];
		//індекс для масиву "sub_indexes"
		int sub_ind_count = 0;
		//перевіряю чи працювала із вибраною триграмою
		if (!(find(substrs.begin(), substrs.end(), cipher.substr(i, 3)) != substrs.end()))
		{
			//не працювала. отже, додаю її до списку аби при повторному її отриманні
			//не перераховувати її дистанцію знов
			substrs.push_back(cipher.substr(i, 3));
			//шукаю триграму в шифрі
			for (int i1 = 0; i1 < cipher.size() - 2; i1++)
			{
				//якщо відстаней 3, то...
				//(вистачить 3 відстані між триграмами аби вирахувати дистанцію)
				if (sub_ind_count == 2) {
					//вирах дистанцію
					int distance = sub_indexes[1] - sub_indexes[0];
					//додаю дистанцію до вектора
					distances.push_back(distance);

					cout << "substr " << cipher.substr(i, 3) << "  sub_indexes = "
						<< sub_indexes[1] << " and "
						<< sub_indexes[0]
						<< "  distance = " << distance << endl;
					cout << endl;

					break;
				}

				//якщо відстаней в масиві менше 3
				if (cipher.substr(i, 3) == cipher.substr(i1, 3))
				{
					//додаю відстань до масиву відстаней
					//cout << cipher.substr(i, 3) << " - pos = " << i1 << endl;
					sub_indexes[sub_ind_count] = i1;
					//зб. лічильник відстаней
					sub_ind_count++;
				}
			}
		}

	}
//}
	
	return distances;
}

//set for me. get factors without repeating for cheking. you can delete it
set<int> fact;
vector<int> getFactors(vector<int> factors, vector<int> distances)
{
	//беру кожну дистанцію
	for (int i = 0; i < distances.size(); i++)
	{
		//more then 2 because two is too short for keyword length
		//перебираю цифри для отримання фактора. 
		//(так як фактор - це довжина слова-ключа, а довжина слова має
		//сенс, якщо більша за 2, то i1(фактор) має бути більшим за 2
		for (int i1 = distances.at(i); i1 > 2; i1--)
		{
			//вираховую фактор дистанції
			if (distances.at(i) % i1 == 0)
			{
				//ключ слово за мною поставленою умовою може бути менше 20
				if (i1 < 20)
				{
					//додаю отриманий фактор
					factors.push_back(i1);
					//you can delete fact.insert(i1). its too for set
					fact.insert(i1);
				}
			}
		}
	}

	return factors;
}

//по факторам отримаю визначену довжину слова-ключа
int getKeyLength(set<int> lengths, vector<int> factors, string cipher)
{
	vector<int> temp;
	for (int i = 0; i < factors.size(); i++)
	{
		int count_factors = 0;
		if (!(find(temp.begin(), temp.end(), factors.at(i)) != temp.end())) {
			temp.push_back(factors.at(i));

			for (int i1 = 0; i1 < factors.size(); i1++)
			{				
				if (factors.at(i) == factors.at(i1))
					count_factors++;
			}
			//додаю значення фактора і його к-ть в шифрі
			keyLengths.push_back(make_pair(count_factors, factors.at(i)));
		}
	}
	
	//сортую аби зручніше знайти довжини
	sort(keyLengths.begin(), keyLengths.end());
	
	cout << "count of factors " << "  probable length" << endl;
	
	for (int i2 = keyLengths.size()-1; i2 > 0; i2--)
	{
		//беру три фактори(довжини), яких найбільше в масиві
		if (lengths.size() >= 3) break;
		lengths.insert(keyLengths[i2].second);
		cout << keyLengths[i2].first << ", " << keyLengths[i2].second << endl;
	}

	
	vector<char> vIn;

	//для остаточного отримання довжини слова знаходжу кофефіцієнт співпадіння

	//перебираю довжини
	for (set<int>::iterator i = lengths.begin(); i != lengths.end(); i++) {
		int element = *i;
		cout << *i << " ";
		int len = *i;
		//передаю отриману довжину в ф-цію для вирахування індекса співпадіння
		res.push_back(make_pair(getIC(len, cipher), len));
	}

	//sort(res.begin(), res.end());
	using pair_type = decltype(res)::value_type;
	auto pr = max_element
	(
		//шукаю найбільший індекс співпадіння
		begin(res), end(res),
		[](const pair_type & p1, const pair_type & p2) {
		return p1.first < p2.first;
	}
	);
	cout << "Value of vector: " << pr->first << '\n' << pr->second;

	return pr->second;
}

//іксова строка із і-тими елементами шифра
vector<char> Gx;
vector<char> stopDublicate;

float getIC(int len, string cipher)
{
	float IC = 0;

	for (int i = 0; i < len; i++)
	{
		cout << i << ": ";
		for (int i1 = i; i1 < cipher.size(); i1 += len)
		{
			cout << cipher.at(i1);
			Gx.push_back(cipher.at(i1));
		}
		cout << endl << "------------------" << endl;
		
		int IC_1 = 0;
		for (int i2 = 0; i2 < Gx.size(); i2++)
		{
			if (!(find(stopDublicate.begin(), stopDublicate.end(), Gx.at(i2)) != stopDublicate.end()))
			{
				int count = 0;
				stopDublicate.push_back(Gx.at(i2));
				for (int i3 = 0; i3 < Gx.size(); i3++)
				{
					if (Gx.at(i2) == Gx.at(i3)) {
						count++;
					}
				}
				cout << Gx.at(i2) << " count = " << count << endl;
				//times of repeating char
				int F = count;
				IC_1 += (F * (F - 1));
			}		
		}
		
		//отримую к-ть символів в G
		int N = Gx.size();
		//отримую індекс поточної літери
		IC += (float)IC_1 / (float)(N * (N - 1));
		cout << endl << "---- IC ----- " << endl << len << " IC = " << IC << endl;
		
		Gx.clear();
		stopDublicate.clear();
	}
	//отримую індекс всіх літер
	float res = IC / len;
	cout << endl << "res = " << res << endl;
	//зачищаю для подальшої роботи із цими масивами
	Gx.clear();
	stopDublicate.clear();

	return res;
}

void getAllX2(string cipher, int length)
{
	cout << endl << "get Gx: " << endl;
	
	//get Gx
	for (int i = 0; i < length; i++)
	{
		cout << i + 1 << ": ";

		//G1, G2...
		for (int i1 = i; i1 < cipher.size(); i1 += length)
		{
			cout << cipher.at(i1);
			Gx.push_back(cipher.at(i1));
		}
		cout << endl << "------------------" << endl;
		vector<char> vv;
		int newPos;
		float x2 = 0;
		list_x2 list1;
		list1.clear();

		for (int i2 = 0; i2 < 26; i2++)
		{
			
			for (int i3 = 0; i3 < Gx.size(); i3++)
			{
				char *pch = strchr(engAlphab, Gx.at(i3));
				while (pch != NULL)
				{
					//printf("found at %d\n", pch - engAlphab + 1);
					int pos = pch - engAlphab;
					newPos = pos - i2;
					if (newPos < 0) newPos = 25;
					pch = strchr(pch + 1, Gx.at(i3));
				}
				vv.push_back(engAlphab[newPos]);
			}
				x2 = getX2(vv);
				list1.insert(list_x2::value_type(engAlphab[i2], x2));
				vv.clear();
		}
	}
	//вибираю із списку з усіма отриманими Х2 найменші Х2. У мапі list1 навпроти кожного такого ікса стоїть буква
	//ця буква і є буквою слова-ключа
	//тепер за допомогою ключа і квадрата отримую висхідний текст
	
}

float getX2(vector<char> Gx)
{
	float x2 = 0;
	float f = 0;
	//беру кожну букву і рахую її к-ть в цьому ж масиві
	for (int i2 = 0; i2 < Gx.size(); i2++)
	{
		if (!(find(stopDublicate.begin(), stopDublicate.end(), Gx.at(i2)) != stopDublicate.end()))
		{
			int count = 0;
			stopDublicate.push_back(Gx.at(i2));
			//перебираю всі букви масиву
			for (int i3 = 0; i3 < Gx.size(); i3++)
			{
				if (Gx.at(i2) == Gx.at(i3)) {
					count++;
				}
			}
			//cout << Gx.at(i2) << " count = " << count << endl;

			f = ((float)count / (float)Gx.size());
			int FF;
			char *pch = strchr(engAlphab, Gx.at(i2));
			while (pch != NULL)
			{
				//printf("found at %d\n", pch - engAlphab + 1);
				FF = pch - engAlphab;
				pch = strchr(pch + 1, Gx.at(i2));
			}
			float F = F_engFreqVal[FF];
			x2 += (float)pow((f - F), 2) / F;

		}

	}
	cout << endl << "x2 = " << x2;

	Gx.clear();
	stopDublicate.clear();

	return x2;
}

