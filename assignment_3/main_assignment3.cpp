// created by Hriday Agrawal
/*
- For running the program, replace the url (http://127.0.0.1:8000/users) with your API endpoint.
- Run the program using:
~$ g++ main_assignment3.cpp -l curl
~$ ./a.out

Input JSON:
===========
[
    {"name": "Frank", "email": "frank@example.com", "phone": "444-555-6666", "age": 50},
    {"name": "Alice", "email": "alice@example.com", "phone": "123-456-7890", "age": 25},
    {"name": "Bob", "email": "bob@example.com", "phone": "987-654-3210", "age": 30},
    {"name": "Charlie", "email": "charlie@example.com", "phone": "555-555-5555", "age": 35},
    {"name": "David", "email": "david@example.com", "phone": "111-222-3333", "age": 40},
    {"name": "Eve", "email": "eve@example.com", "phone": "999-888-7777", "age": 0},
    {"name": "Grace", "email": "grace@example.com", "phone": "777-666-5555", "age": 55},
    {"name": "Henry", "email": "henry@example.com", "phone": "666-777-8888", "age": 60},
    {"name": "Ivy", "email": "ivy@example.com", "phone": "888-999-0000", "age": 65},
    {"name": "Jack", "email": "jack@example.com", "phone": "123-987-6543", "age": 70}

]



OUTPUT
======
Name: Alice, Age: 25, Email: alice@example.com, Phone No.: 123-456-7890
Name: Bob, Age: 30, Email: bob@example.com, Phone No.: 987-654-3210
Name: Charlie, Age: 35, Email: charlie@example.com, Phone No.: 555-555-5555
Name: David, Age: 40, Email: david@example.com, Phone No.: 111-222-3333
Name: Eve, Age: Invalid Age, Email: eve@example.com, Phone No.: 999-888-7777
Name: Frank, Age: 50, Email: frank@example.com, Phone No.: 444-555-6666
Name: Grace, Age: 55, Email: grace@example.com, Phone No.: 777-666-5555
Name: Henry, Age: 60, Email: henry@example.com, Phone No.: 666-777-8888
Name: Ivy, Age: 65, Email: ivy@example.com, Phone No.: 888-999-0000
Name: Jack, Age: 70, Email: jack@example.com, Phone No.: 123-987-6543

*/

#include <bits/stdc++.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

class TrieNode
{
public:
    unordered_map<char, TrieNode *> children;
    bool isEndOfWord;
    int age = 0;
    string email = "Person doesn't exist";
    string phone = "Person doesn't exist";

    TrieNode()
    {
        isEndOfWord = false;
    }
};

class Trie
{
private:
    TrieNode *root;

    void dfs(TrieNode *node, string prefix, vector<string> &results)
    {
        if (node == nullptr)
            return;
        if (node->isEndOfWord)
            results.push_back(prefix);

        for (const auto &child : node->children)
        {
            dfs(child.second, prefix + child.first, results);
        }
    }

public:
    Trie()
    {
        root = new TrieNode();
    }

    // Insert a word into the trie
    void insert(string word, int years, string mail, string phoneNum)
    {
        TrieNode *current = root;
        for (char c : word)
        {
            if (current->children.find(c) == current->children.end())
            {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
        current->age = years;
        current->phone = phoneNum;
        current->email = mail;
    }

    // Search for a word in the trie
    TrieNode *search(string word)
    {
        TrieNode *current = root;
        for (char c : word)
        {
            if (current->children.find(c) == current->children.end())
            {
                current->isEndOfWord = false;
                return current;
            }
            current = current->children[c];
        }
        return current;
    }

    // Check if any word in the trie starts with the given prefix
    bool startsWith(string prefix)
    {
        TrieNode *current = root;
        for (char c : prefix)
        {
            if (current->children.find(c) == current->children.end())
            {
                return false;
            }
            current = current->children[c];
        }
        return true;
    }

    // Generating a vector of sorted users
    vector<string> getSortedWords()
    {
        vector<string> results;
        dfs(root, "", results);
        sort(results.begin(), results.end());
        return results;
    }
};

// Structure to hold user details
struct UserDetail
{
    string name;
    string email;
    string phone;
    int age;
};

// callback function to handle response data
size_t write_callback(void *ptr, size_t size, size_t nmemb, string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

int main()
{
    // initializing datastructure
    Trie trie;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();

    if (curl)
    {
        // Set the URL for the request
        string url = "http://127.0.0.1:8000/users"; // Change URL as needed
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set up a string to hold the response data
        string response_data;

        // Set the write callback function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            cerr << "Failed to perform HTTP request: " << curl_easy_strerror(res) << endl;
        }
        else
        {
            // Parse JSON response
            json json_data = json::parse(response_data);

            // Convert JSON data to vector of UserDetail structures
            vector<UserDetail> users;
            for (const auto &user : json_data)
            {
                UserDetail u;
                u.name = user["name"];
                u.email = user["email"];
                u.phone = user["phone"];
                u.age = user["age"];
                users.push_back(u);
            }

            for (auto i : users)
            {
                trie.insert(i.name, i.age, i.email, i.phone);
            }

            // Generating a vector of sorted users (using search functionality)
            vector<string> sortedWords = trie.getSortedWords();
            vector<string> sortedUsers;

            for (const string &word : sortedWords)
            {
                TrieNode *a = trie.search(word);
                string user = "Name: " + word + ", Age: " + (a->age ? to_string(a->age) : "Invalid Age") + ", Email: " + a->email + ", Phone No.: " + a->phone + '\n';
                sortedUsers.push_back(user);
            }

            // Writing the sorted users in another text file
            ofstream ofile;
            ofile.open("sortedUsers.txt", ios::app);
            for (string i : sortedUsers)
            {
                cout << i;
            }
        }

        // Clean up
        curl_easy_cleanup(curl);
    }
    else
    {
        cerr << "Failed to initialize libcurl." << endl;
    }

    // Clean up libcurl
    curl_global_cleanup();

    return 0;
}
