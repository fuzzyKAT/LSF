#ifndef WORD_ITERATOR_H_
#define WORD_ITERATOR_H_
#include <map>
#include <string>

using std::string;

class WordIterator
{
public:

    bool validateWord(const string &word)
    {
        return (word.find_first_not_of(operators) == string::npos);
    }
    
    // Добавляет порождающее правило
    void addRule(char var, string &rule)
    {
        rules.insert(std::pair<char, string>(var, rule));
    }
    
    // Итерирует строку axiom
    string iterateWord(const string &axiom)
    {
        string word;
        
        string::const_iterator i;
        for(i = axiom.begin(); i != axiom.end(); ++i)
        {
            switch(*i)
            {
                case '\n' :
                case '\r' :
                case ' ' :
                    break;
                //case 'F' :
                //case 'b' : 
                case '+' :
                case '-' :
                case '[' : 
                case ']' : 
                    word.push_back(*i);
                    break;
                
                default : 
                {
                    std::map<char, string>::iterator it = rules.find(*i);
                    
                    if(it == rules.end())
                    {
                        word.push_back(*i);
                        break;
                    }
                    
                    //string &rule = rules[*i];
                    word += (*it).second;
                    break;
                }
            }
        }
        
        return word;
    }
    
private:

    // Набор порождающих правил 
    std::map<char, string> rules;
    
    // Набор операторов L-системы
    const char *operators = "Fb+=[]";
};

#endif