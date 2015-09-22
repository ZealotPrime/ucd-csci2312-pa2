#include <iostream>
#include "Point.h"

using namespace std;

void basicPointOps()
{
    Clustering::Point testPoint(3);
    Clustering::Point testPoint2(3);

    testPoint.setValue(0,1);
    testPoint.setValue(1,2);
    testPoint.setValue(2,3);

    testPoint2.setValue(0,1);
    testPoint2.setValue(1,2);
    testPoint2.setValue(2,3);

    cout<<testPoint<<endl;
    cout<<testPoint2<<endl;

    if(testPoint==testPoint2)
        cout<<"Points are equal"<<endl;
    if(testPoint!=testPoint2)
        cout<<"Points are not equal"<<endl;

    testPoint+=testPoint2;

    cout<<testPoint<<endl;
    cout<<testPoint2<<endl;
    if(testPoint==testPoint2)
        cout<<"Points are equal"<<endl;
    if(testPoint!=testPoint2)
        cout<<"Points are not equal"<<endl;

    testPoint2*=2;
    cout<<testPoint2<<endl;
    testPoint2/=2;
    cout<<testPoint2<<endl;


}

int main() {
    basicPointOps();
    return 0;
}