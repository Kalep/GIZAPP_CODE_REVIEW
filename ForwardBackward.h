/*

Copyright (C) 1999,2000,2001  Franz Josef Och (RWTH Aachen - Lehrstuhl fuer Informatik VI)

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
#ifndef NO_EM_MARKOF_ZEUGS_DEFINED
#define NO_EM_MARKOF_ZEUGS_DEFINED
#ifndef NO_TRAINING
#include "myassert.h"
#include "Array.h"
#include "Array2.h"

class HMMNetwork
{
public:
    /**源端长度的两倍*/
    int as;
    /**目标端的长度*/
    int bs;
    /**一个as*bs的二维数组*/
    Array2<double> n;
    /**一个3维数组*/
    Array<Array2<double> > e;
    /**初始的alpha概率*/
    Array<double> alphainit;
    /**初始的beta概率*/
    Array<double> betainit;
    /**as*bs的一个长度*/
    int ab;
    /**一个乘子,用来算熵的*/
    double finalMultiply;
    /**
     * 构造函数
     * @param I 源端的长度的两倍
     * @param J 目标端的长度
     */
    HMMNetwork(int I, int J)
        : as(I), bs(J), \
        n(as, bs),/*e(as,as,0.0),*/e(0),\
        alphainit(as, 1.0 / as), betainit(as, 1.0), \
        ab(as *bs), \
        finalMultiply(1.0)
    {}
    /**
     * 获得alpha的初始值
     * @param  i  源端下标
     * @return   alpha的概率值，初始为1/as
     */
    double getAlphainit(int i) const
    {
        return alphainit[i];
    }
    /**
     * 获得beta的初始值
     * @param  i 目标端下标
     * @return   beta的初始值
     */
    double getBetainit(int i) const
    {
        return betainit[i];
    }
    /**
     * 获得第一维的长度
     * @return 源端长度的两倍
     */
    inline int size1()const
    {
        return as;
    }
    /**
     * 获得第二维的长度
     * @return 目标端长度
     */
    inline int size2()const
    {
        return bs;
    }
    /**
     * 获得某一个节点的概率
     * @param  i 源端的下标
     * @param  j 目标端的下标
     * @return   节点的概率
     */
    inline const double &nodeProb(int i, int j)const
    {
        return n(i, j);
    }
    /**
     * 获得输出概率
     * @param  j  目标端的下标
     * @param  i1 源端的下标1
     * @param  i2 源端的下标2
     * @return    输出概率
     */
    inline const double &outProb(int j, int i1, int i2)const
    {
      /*massert(e[min(int(e.size())-1,j)](i1,i2) );*/ 
      return e[min(int(e.size()) - 1, j)](i1, i2);
    }
    /**
     * 重载输出流
     */
    friend ostream &operator<<(ostream &out, const HMMNetwork &x)
    {
        return out << "N: \n" << x.n << endl << "E: \n" << x.e << "A:\n" << x.alphainit << "B:\n" << x.betainit << endl;
    }
};
double ForwardBackwardTraining(const HMMNetwork &mc, Array<double> &gamma, Array<Array2<double> > &epsilon);
void HMMViterbi(const HMMNetwork &mc, Array<int> &vit);
double HMMRealViterbi(const HMMNetwork &net, Array<int> &vit, int pegi = -1, int pegj = -1, bool verbose = 0);
double MaximumTraining(const HMMNetwork &net, Array<double> &g, Array<Array2<double> > &e);
void HMMViterbi(const HMMNetwork &net, Array<double> &g, Array<int> &vit);
#endif
#endif
