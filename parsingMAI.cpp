#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <curl/curl.h>
#include <regex>

using namespace std;

// Функция для загрузки содержимого веб-страницы
size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string* str = (string*)userdata;
    str->append(ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    string url = "https://mai.ru/education/studies/schedule/groups.php";
    string html;

    // Инициализация libcurl
    curl = curl_easy_init();
    if (curl) {
        // Установка URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Установка функции обратного вызова для записи данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        // Установка пользовательских данных для функции обратного вызова
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

        // Выполнение запроса
        res = curl_easy_perform(curl);

        // Проверка результата запроса
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }

        // Очистка libcurl
        curl_easy_cleanup(curl);
    }

    // Регулярное выражение для поиска расписания
    regex pattern(R"(<div class="schedule-item">(.*?)</div>)");
    smatch match;
    vector<string> schedule;

    // Поиск расписания на странице
    while (regex_search(html, match, pattern)) {
        schedule.push_back(match[1]);
        html = match.suffix();
    }

    // Вывод расписания
    for (const auto& item : schedule) {
        cout << item << endl << endl;
    }

    return 0;
}