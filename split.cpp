#include <vector>
#include <string>
#include <cctype> // isspace を定義している
#include "split.h"

using std::vector;  using std::string;
using std::isspace;

// 引数が空白ならtrueそうでなけらばfalse
bool space(char c)
{
  return isspace(c);
}

// 引数が空白ならfalse、そうでなければtrue
bool not_space(char c)
{
  return !isspace(c);
}

vector<string> split(const string& str)
{
  typedef string::const_iterator iter;
  vector<string> ret;
  iter i = str.begin();
  while (i != str.end()) {
    // 始めに空白があればそれらは無視する
    i = find_if(i, str.end(), not_space);
    // 次の単語の終わりを探す
    iter j = find_if(i, str.end(), space);
    // [i, j)の範囲の文字をコピー
    if (i != str.end())
      ret.push_back(string(i, j));
    i = j;
  }
  return ret;
}
