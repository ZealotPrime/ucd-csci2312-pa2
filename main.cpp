#include <iostream>
#include "Point.h"
#include "Clustering.h"

using namespace std;

void pointTester()
{
    Clustering::Point testPoint(3);
    Clustering::Point testPoint2(3);

    testPoint.setValue(0,1);
    testPoint.setValue(1,2);
    testPoint.setValue(2,3);

    testPoint2.setValue(0,1);
    testPoint2.setValue(1,2);
    testPoint2.setValue(2,3);

    cout<<"two equal points created"<<endl;

    cout<<testPoint<<endl;//print test
    cout<<testPoint2<<endl;

    cout<<endl<<"test for equality when equal"<<endl;
    if(testPoint==testPoint2)
        cout<<"Points are equal"<<endl;
    if(testPoint!=testPoint2)
        cout<<"Points are not equal"<<endl;

    cout<<endl<<"point2 added to point 1"<<endl;
    testPoint+=testPoint2;
    cout<<testPoint<<endl;
    cout<<testPoint2<<endl;

    cout<<endl<<"get value of testpoint[3]"<<endl;
    cout<<testPoint[3]<<endl;

    cout<<endl<<"test for equality when not equal"<<endl;
    if(testPoint==testPoint2)
        cout<<"Points are equal"<<endl;
    if(testPoint!=testPoint2)
        cout<<"Points are not equal"<<endl;

    cout<<endl<<"multiply then divide testpoint2 by 2"<<endl;
    testPoint2*=2;//test mult/div ops
    cout<<testPoint2<<endl;
    testPoint2/=2;
    cout<<testPoint2<<endl;

    cout<<endl<<"test comparators when not equal"<<endl;
    if(testPoint>testPoint2)
        cout<<"Test point greater"<<endl;
    if(testPoint<testPoint2)
        cout<<"Test point 2 greater"<<endl;
    if(testPoint>=testPoint2)
        cout<<"Test point greater or equal"<<endl;
    if(testPoint<=testPoint2)
        cout<<"Test point 2 greater or equal"<<endl;

    cout<<endl<<"make them equal again and test comparators again"<<endl;
    testPoint=testPoint2;
    if(testPoint>testPoint2)//test comparators when equal
        cout<<"Test point greater"<<endl;
    if(testPoint<testPoint2)
        cout<<"Test point 2 greater"<<endl;
    if(testPoint>=testPoint2)
        cout<<"Test point greater or equal"<<endl;
    if(testPoint<=testPoint2)
        cout<<"Test point 2 greater or equal"<<endl;

    cout<<endl<<"Add them together :"<<testPoint+testPoint2<<endl;
    cout<<endl<<"subtract them :"<<testPoint-testPoint2<<endl;

    //cout<<"Now enter 3 new dim values for test point 2 to test steam extraction operator:"<<endl;
    //cin>>testPoint2;
    //cout<<"point is now "<<testPoint2;

    cout<<"Testing comparators again with new values"<<endl;
    if(testPoint>testPoint2)
        cout<<"Test point greater"<<endl;
    if(testPoint<testPoint2)
        cout<<"Test point 2 greater"<<endl;
    if(testPoint>=testPoint2)
        cout<<"Test point greater or equal"<<endl;
    if(testPoint<=testPoint2)
        cout<<"Test point 2 greater or equal"<<endl;

}//end point tester

void clusterTester()
{
    double set1[]={1,2,3};
    double set2[]={5,6,7};
    Clustering::PointPtr p1=new Clustering::Point(3,set1);
    Clustering::PointPtr p2=new Clustering::Point(3,set2);
    Clustering::PointPtr p3=new Clustering::Point(3,set2);
    *p3*=2;
    Clustering::Cluster c1;

    cout<<c1<<endl;
    cout<<"Adding a new point to c1 cluster"<<endl;
    c1.add(p1);
    cout<<c1<<endl;
    cout<<"Adding another 2 points"<<endl;
    c1.add(p3);
    c1.add(p2);
    cout<<c1;
    cout<<"Now removing p2"<<endl;
    c1.remove(p2);
    cout<<c1;
    cout<<"creating another cluster with c1"<<endl;
    Clustering::Cluster c2(c1);
    cout<<c1<<c2;
    cout<<"removing points from c2"<<endl;
    c2.remove(p1);
    c2.remove(p3);
    c1.add(p2);
    cout<<c1<<c2<<"+Equality op test"<<endl;
    c2=c1;
    cout<<c1<<c2;

}


int main() {
    pointTester();
    clusterTester();
    return 0;
}