#include <iostream>
#include <bitset>
#include <compare>
#include <vector>

//#define _NSTD_LONGINT_DEBUGGING_

#include "LongInt.h"
#include "Machine_Code_Emulator.h"
//#include "Deque.h"
//#include "Timer.h"

_NSTD_UNSIGNED
using namespace std;
#include <algorithm>
/*
class Solution {
public:
    static bool isMatch(string s, string p) {
        for (int i = p.find('*'); i != string::npos && p.find('*', i) != string::npos; i = p.find('*', i + 1))
            remove(p, p.substr(i - 1, 2), i + 1);
        return checkRec(s, splitRec(p));
    }
//private:
    struct token {
        enum Type { STRING, LONG, ANY, LONG_ANY };
        token(string s, Type t) : s_(s),  t_(t) {}
        string s_;
        Type t_;
    };
    static bool checkRec(string s, const vector<token>& pv, int index = 0) {
        for (int i = index; i < pv.size(); i++) {
            const token& t = pv[i];
            switch (t.t_) {
            case token::Type::STRING:
                if (s.length() < t.s_.length() || !is_eq(s.substr(0, t.s_.length()), t.s_))
                    return false;
                s = s.substr(t.s_.length());
                break;
            case token::Type::ANY:
                if (!s.length())
                    return false;
                s = s.substr(1);
                break;
            case token::Type::LONG_ANY:
                for (; s.length(); s = s.substr(1))
                    if (checkRec(s, pv, i + 1))
                        return true;
                break;
            case token::Type::LONG:
                for (; s.length() && s[0] == t.s_[0]; s = s.substr(1))
                    if (checkRec(s, pv, i + 1))
                        return true;
                break;
            }
        }
        return !s.length();
    }
    static vector<token> splitRec(string s) {
        vector<token> out;
        if (!s.length())
            return out;
        if (contains(s, '*')) {
            auto v = splitRec(s.substr(0, s.find('*') - 1));
            out.insert(out.end(), v.begin(), v.end());
            out.push_back(token(string({ s[s.find('*') - 1], '*' }), s[s.find('*') - 1] == '.' ? token::Type::LONG_ANY : token::Type::LONG));
            v = splitRec(s.substr(s.find('*') + 1));
            out.insert(out.end(), v.begin(), v.end());
            return out;
        }
        if (contains(s, '.')) {
            auto v = splitRec(s.substr(0,s.find('.')));
            out.insert(out.end(), v.begin(), v.end());
            out.push_back(token(".", token::Type::ANY));
            v = splitRec(s.substr(s.find('.') + 1));
            out.insert(out.end(), v.begin(), v.end());
            return out;
        }
        out.push_back(token(s, token::Type::STRING));
        return out;
    }
    static bool contains(const string& s, char c) {
        for (const char& c1 : s)
            if (c1 == c)
                return true;
        return false;
    }
    static void remove(string& s, const string& str, int pos = 0) {
        for (int i = pos; i < s.length() - str.length(); i += str.length())
            if (is_eq(s.substr(i, str.length()), str))
                s.erase(s.begin() + i, s.begin() + i + str.length());
            else
                break;
    }
    static bool is_eq(const string& a, const string& b) {
        if (a.length() != b.length())
            return false;
        for (int i = 0; i < a.length(); i++)
            if (a[i] != b[i])
                return false;
        return true;
    }
};
*/
class Solution {
public:
    static vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> out;
        for (int i = 0; words.size() * words[0].length() <= s.length(); s = s.substr(1), i++)
            if (test(s, words))
                out.push_back(i);
        return out;
    }
    static bool test(string a, vector<string> vs) {
        if (!vs.size())
            return true;
        else if (!a.length())
            return false;
        for (int i = 0; i < vs.size(); i++)
            if (comp_str(a, vs[i])) {
                a = a.substr(vs[i].length());
                vs.erase(vs.begin() + i);
                i = 0;
            }
        return !vs.size();
    }
    static bool comp_str(const string& a, const string& b) {
        cout << '\n' << a << " v " << b;
        for (int i = 0; i < b.length(); i++)
            if (a[i] != b[i]) {
                cout << "\tf";
                return false;
            }
        cout << "\tt\n";
        return true;
    }
};

void psvec(vector<string> vs) {
    for (string s : vs)
        cout << '\"' << s << '\"' << ' ';
}

int main()
{
    nstd::LongInt li(100);
    nstd::LongInt li2 = li + 60093;
    _STD cout << "addition succeeded";
//    _STD cout << static_cast<uint>(li % 10); << " " << (static_cast<uint>(li) % 10);

/*  vector<string> s({"word", "good", "best", "good"});
    auto v = Solution::findSubstring("wordgoodgoodgoodbestword", s);
    if (!v.size())
        cout << "\nn";
    for (int i : v)
        cout << i << '\t';
*/
	std::cout << '\n';
	std::system("pause");
}