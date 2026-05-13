#include <iostream>
using namespace std;

class Solution
{
public:
long long getSolved(int N,int from,int to,int aux,long long &count) // main func
    {
        if(N==0)
        {
            return 0; // base case 
        }
        ++count; // counts the move
        getSolved(N-1,from,aux,to,count); // moves N-1 disks from src to aux
        cout<<"move disk "<<N<<" from rod "<<from<<" to rod "<<to<<endl;
        getSolved(N-1,aux,to,from,count); // moves N-1 disks from aux to dest
        return count;
    }
    long long amp(int N, int from, int to, int aux) { // wrapper func
        long long count=0;
        getSolved(N,from,to,aux,count);
        return count;
    }
   }; 


int main()
{
    Solution obj;
    int N;

    cout << "Enter number of disks: ";
    cin >> N;   

    long long totalMoves = obj.amp(N, 1, 3, 2);
    cout << "\nTotal moves = " << totalMoves << endl;

    return 0;
}