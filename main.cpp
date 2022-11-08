#include <vector>
#include <iostream>
#include <bitset>

using namespace std;

//Taken from: https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup
static const char LogTable256[256] = 
{
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
    LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};


inline int most_significant_bit(int v){
    int r;
    int t, tt = v >> 16; // temporaries
    if (tt)
    {
        r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
    }
    else 
    {
        r = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
    }
    return r;
}

//----------------------------------------------------------------------------

int main()
{
    //Palindromes generation: TODO: less ugly!!!
    vector<vector<int>> palindromesByDigits;
    palindromesByDigits.push_back(vector<int>()); //0 digits
    palindromesByDigits.push_back(vector<int>()); //1 digit

    for(int i = 0; i < 10; i ++){
        palindromesByDigits[1].push_back(i);
    }

    int ndigpal = 2;
    int ndigsuf = 1;
    int prevmult = 1;
    int mult = 10;
    int multp1 = 100;
    while(mult <= 1000){
        palindromesByDigits.push_back(vector<int>()); //ndig digits
        palindromesByDigits.push_back(vector<int>()); //ndig+1 digits
        for(int i = prevmult; i < mult; i++){
            int irev=0, imulti = i, d;
            for(int j = 0; j < ndigsuf; j++){
                d = imulti%10;
                imulti /=10;
                irev = 10*irev + d;
            }
            palindromesByDigits[ndigpal].push_back(i*mult + irev);
            imulti = i*multp1;
            if(ndigpal == 6) continue; //Skip 7 digits palindromes generation
            for(int k = 0; k < multp1; k+=mult){
                palindromesByDigits[ndigpal+1].push_back(imulti+k+irev);
            }
        }
        ndigsuf++;
        ndigpal+=2;
        prevmult = mult;
        mult = multp1;
        multp1*=10;
    }
    
    //Iterate base10 palindromes and check base2 palindromity
    int ans = 0;
    for(int ndig = 1; ndig < 7; ndig++){
        for(int &d:palindromesByDigits[ndig]){
            int leftbitmask = (1<<most_significant_bit(d));
            int rightbitmask = 1;
            bool ispalindrome = true;
            while(ispalindrome && leftbitmask > rightbitmask)
            {
                ispalindrome = (!(d&leftbitmask)) == (!(d&rightbitmask));
                rightbitmask <<=1;
                leftbitmask >>=1;
            }
            if(ispalindrome){
                ans += d;
            }
        }
    }
    cout << ans << endl;
}