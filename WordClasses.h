/*

Copyright (C) 2000,2001  Franz Josef Och (RWTH Aachen - Lehrstuhl fuer Informatik VI)

This file is part of GIZA++ ( extension of GIZA ).

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.

*/
#ifndef WordClasses_h_DEFINED
#define WordClasses_h_DEFINED
#include <map>
#include <string>
#include <set>

class WordClasses
{
private:
    //first是word的字符串表示，second是到class的字符串表示
    map<string, string> Sw2c;
    //first是classes的label,second是classes的数字化id
    map<string, int> Sc2int;
    //下标是类的数字化id, 内容是类的label,这样可以跟聚类系统区分开来
    Vector<string> Sint2c;
    //下标是word的数字化id,内容是classes的数字化id,代表是第几出现的
    Vector<int> w2c; 
    //记录有多少个类
    unsigned int classes;
public:
    /**
     * 词的类别
     */
    WordClasses()
        : classes(1)
    {
        Sint2c.push_back("0");
        Sc2int["0"] = 0;
    }
    /**
     * 读取class文件
     * @param  in class文件的输入流
     * @param  m  word的序列化
     * @return    读取是否成功
     */
    template<class MAPPER> bool read(istream &in, const MAPPER &m)
    {
        string sline;
        int maxword = 0;
        while (getline(in, sline))
        {
            string word, wclass;
            //一个单词，一个类型
            //istringstream iline(sline.c_str());
            istringstream iline(sline);
            iline >> word >> wclass;
            //m(word) 取的是id
            maxword = max(m(word), maxword);
            //这个词没有在其他的类出现过
            assert(Sw2c.count(word) == 0);
            // word class
            Sw2c[word] = wclass;

            if ( !Sc2int.count(wclass) )
            {   //映射到序号关系
                Sc2int[wclass] = classes++;
                //记录一下
                Sint2c.push_back(wclass);

                assert(classes == Sint2c.size());
            }
        }
        w2c = Vector<int>(maxword + 1, 0);
        for (map<string, string>::const_iterator i = Sw2c.begin(); i != Sw2c.end(); ++i)
            w2c[m(i->first)] = Sc2int[i->second];
        cout << "Read classes: #words: " << maxword << " " << " #classes: " << classes << endl;
        return 1;
    }
    /**
     * 获得word对应的类的序号
     * @param w  word的id
     * @return word对应的class的序号，并非label
     */
    int getClass(int w)const
    {
        if (w >= 0 && int(w) < int(w2c.size()) )
            return w2c[w];
        else
            return 0;
    }
    /**
     * 根据类的label,获得类的序号
     * @param  x 类的label
     * @return   类的序号
     */
    int operator()(const string &x) const
    {
        if ( Sc2int.count(x) )
            return Sc2int.find(x)->second;
        else
        {
            cerr << "WARNING:  class " << x << " not found.\n";
            return 0;
        }
    }
    //根据序号取类
    /**
     * 根据类的序号返回类的label
     * @param  cnr 类的序号
     * @return     类的label
     */
    string classString(unsigned int cnr)const
    {
        if ( cnr < Sint2c.size())
            return Sint2c[cnr];
        else
            return string("0");
    }
};

#endif
