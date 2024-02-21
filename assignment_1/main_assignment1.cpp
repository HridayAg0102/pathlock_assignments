// created by Hriday Agrawal
#include <bits/stdc++.h>
using namespace std;

class TrieNode
{
public:
    unordered_map<char, TrieNode *> children;
    bool isEndOfWord;
    string age = "Person doesn't exist";

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
    void insert(string word, string years)
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

int main()
{
    vector<vector<string>> users;

    // file reading
    string line;
    ifstream file("./details.txt");
    while (getline(file, line))
    {
        vector<string> usr;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ','))
        {
            usr.push_back(token);
        };

        users.push_back(usr);
    };

    file.close();

    // insertion
    Trie trie;
    for (int i = 0; i < users.size(); i++)
    {
        trie.insert(users[i][0], users[i][1]);
    }

    TrieNode *a = trie.search("Olivia");
    cout << a->isEndOfWord << a->age << endl; // Output: Found

    // Generating a vector of sorted users (using search functionality)
    vector<string> sortedWords = trie.getSortedWords();
    vector<string> sortedUsers;

    for (const string &word : sortedWords)
    {
        TrieNode *a = trie.search(word);
        string user = word + ',' + a->age;
        sortedUsers.push_back(user);
    }

    // Writing the sorted users in another text file
    ofstream ofile;
    ofile.open("sortedUsers.txt", ios::app);

    for (string i : sortedUsers)
    {
        ofile << i << "\n";
    }

    file.close();

    return 0;
}
