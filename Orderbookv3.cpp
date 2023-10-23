#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <iomanip>
#include <fstream>
using namespace std;
// #define int long long


//for displaying a vector
template <typename T> void disp(vector<T> v){
    for(auto thing : v){
        cout<<thing<<" ";
    }
    cout<<endl;
}

//for displaying a vector
template <typename T, typename U> void disp(vector<pair<T,U>> v){
    for(auto thing : v){
        cout<<thing.first<<" "<<thing.second<<endl;
    }
    cout<<endl;
}


// output.txt is the output file where all the orders and matches are written
void clear(){
    freopen("output.txt", "w",stdout); 
}

// time (increases by 1 after every bid or ask)
int64_t timeNumber = 0; 

// buyOrder class: Object of this class represents a buy order
class buyOrder{ 
    public:
    // string orderID;  
    int64_t64_t orderQuantity; 
    int64_t price;
    int64_t timeNumber;
};

 // sellOrder class: Object of this class represents a sell order
class sellOrder{
    public: 
    // string orderID;
    int64_t orderQuantity;
    int64_t price;
    int64_t timeNumber;
};

 // comparator function for bids priority queue
class fifoBuyComparator{
    public:
    bool operator()(buyOrder A, buyOrder B){
    if(A.price != B.price){
        //greater price, greater priority
        if(A.price < B.price){ 
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(A.timeNumber > B.timeNumber){
             //earlier, more priority
            return true;
        }
        else{
            return false;
        }
    }
}    
};

 // comparator funciton for asks priority queue
class fifoSellComparator{
    public:
    bool operator()(sellOrder A, sellOrder B){
    if(A.price != B.price){
        if(A.price > B.price){ 
            // lesser price, higher priority
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(A.timeNumber > B.timeNumber){
             // earlier, higher priority
            return true; 
        }
        else{
            return false;
        }
    }
}
};

// function to display bids priority queue
void showpq(priority_queue<buyOrder, vector<buyOrder>, fifoBuyComparator> p){
    while(p.size()){
        cout<< p.top().price <<" "<<p.top().orderQuantity<<" "<<p.top().timeNumber<<endl;
        p.pop();
    }
    
}

// overloaded function to display asks priority queue
void showpq(priority_queue<sellOrder, vector<sellOrder>, fifoSellComparator> p){
    while(p.size()){
        cout<<p.top().price<<" "<<p.top().orderQuantity<<" "<<p.top().timeNumber<<endl;
        p.pop();
    }
}

// declaration of the bids priority queue
priority_queue<buyOrder, vector<buyOrder>, fifoBuyComparator> bids; 
// declaration of the asks priority queue
priority_queue<sellOrder, vector<sellOrder>, fifoSellComparator> asks; 

// stores the prices at which orders happened
vector<pair<int64_t,int64_t>> matchedOrders; 


 // displays currently active bids and ask prices, along with matched orders
void showOrderBook(){
    cout<<"Bids :"<<endl;
    if(bids.size()){showpq(bids);}
    cout<<endl;
    cout<<"Asks :"<<endl;
    if(asks.size()){showpq(asks);}
    cout<<endl;
    cout<<"Number of Matched Orders: "<<matchedOrders.size()<<endl;
    cout<<"Matched Order Prices & Quantities: "<<endl;
    if(matchedOrders.size()){disp(matchedOrders);}
}

 // calling this function runs the Fifo matching algorithm. This function is "automatically" run when a buy order/sell order is placed
void matchFifo(){
    clear();
    showOrderBook();
     // so you wont have some access error
    if(bids.size() == 0 || asks.size() == 0){return;}
    // The below Matching Code is for FIFO
    if(bids.top().price == asks.top().price && bids.size() != 0 && asks.size() != 0){
         // checks if any order can happen, i.e. if the price of the best buy order and best sell order match
        int64_t quantityMatched = 0;
        int64_t priceMatched = asks.top().price;
        if(asks.top().orderQuantity > bids.top().orderQuantity){
            while(asks.top().orderQuantity > bids.top().orderQuantity && asks.top().price == bids.top().price){
                auto temp = asks.top();
                temp.orderQuantity -= bids.top().orderQuantity; quantityMatched += bids.top().orderQuantity;
                asks.pop();
                bids.pop();
                asks.push(temp);
                if(bids.size() == 0 || asks.size() == 0){break;}
            }
            if(bids.size() != 0 && asks.size() != 0){
                if(bids.top().orderQuantity > asks.top().orderQuantity && asks.top().price == bids.top().price){
                    auto temp = bids.top();
                    temp.orderQuantity -= asks.top().orderQuantity; quantityMatched += asks.top().orderQuantity;
                    asks.pop();
                    bids.pop();
                    bids.push(temp);
                }
                else if(asks.top().price == bids.top().price){
                    quantityMatched += bids.top().orderQuantity;
                    bids.pop();
                    asks.pop();
                }
            }
        }
        else if(bids.top().orderQuantity > asks.top().orderQuantity){
            while(bids.top().orderQuantity > asks.top().orderQuantity){
                auto temp = bids.top();
                temp.orderQuantity -= asks.top().orderQuantity; quantityMatched += asks.top().orderQuantity;
                asks.pop();
                bids.pop();
                bids.push(temp);
                if(bids.size() == 0 || asks.size() == 0){break;}
            }
            if(bids.size() != 0 && asks.size() != 0){
                if(bids.top().orderQuantity < asks.top().orderQuantity && asks.top().price == bids.top().price){
                    auto temp = asks.top();
                    temp.orderQuantity -= bids.top().orderQuantity; quantityMatched += bids.top().orderQuantity;
                    asks.pop();
                    bids.pop();
                    asks.push(temp);
                }
                else if(asks.top().price == bids.top().price){
                    quantityMatched += bids.top().orderQuantity;
                    bids.pop();
                    asks.pop();
                }
            }
        }
        else{
            quantityMatched = asks.top().orderQuantity;
            asks.pop();
            bids.pop();
        }
        
        matchedOrders.push_back({priceMatched, quantityMatched});
        // // cout<<"Trade Ahoy! : "<<asks.top().price<<endl;                                                                                
        // matchedOrders.push_back(bids.top().price); // then the order is registered to the list of matched orders
        // bids.pop();// the bid price and ask price at which the trade happened, 
        // asks.pop();// are popped form their priority queues  
    }
    clear();
    showOrderBook();
}

 // function that `places` a buy order. It pushes the buy order object int64_to the bids priority queue
void makeBuyOrder(int64_t buyingprice, int64_t orderquantity){
     // this is so that time increases after every order
    timeNumber++;
    //buyOrder object `orderA` created for this order
    buyOrder orderA; 
    // assigning the value of buying price to the object
    orderA.price = buyingprice; 
    // assigning the value of time to this object
    orderA.timeNumber = timeNumber; 
    orderA.orderQuantity = orderquantity;
     // pushing this object int64_to the bids priority queue
    bids.push(orderA);
    matchFifo();
}

// similar to the makeSellOrder function
void makeSellOrder(int64_t sellingprice, int64_t orderquantity){ 
    timeNumber++;
    sellOrder orderA;
    orderA.price = sellingprice;
    orderA.timeNumber = timeNumber;
    orderA.orderQuantity = orderquantity;
    asks.push(orderA);
    matchFifo();
}

// parses the input from user, which is a string
void parse(string s){ 
     // string number
    string stNum = "";
    string stQuantity = "";
    int64_t n = s.length(); int64_t indx = -1;
    for(int64_t i = 1; i<n; i++){
        if(tolower(s[i]) != 'q'){
            stNum.push_back(s[i]);
        }
        else{
            indx = i; break;
        }
    }
    double price = atof(stNum.c_str());
    if(indx == -1){
        stQuantity = "1";
    }
    else{
        for(int64_t i = indx+1; i<n; i++){
            stQuantity.push_back(s[i]);
        }
    }
    int64_t quantity = atoi(stQuantity.c_str());
    if(tolower(s[0]) == 'b'){
        makeBuyOrder(price, quantity);
    }
    else if(tolower(s[0]) == 's'){
        makeSellOrder(price, quantity);
    }
}


#define hi cout<<"HI"<<endl

int64_t main(){
//--------------------
// freopen("input.txt","r",stdin);
freopen("output.txt","w",stdout);
//-------------------
// hi;
// for (int64_t i = 1; i<10; i++){
//     makeBuyOrder(i*i, i);
// }
// makeSellOrder(300,1);
// makeSellOrder(100,20);
// // makeSellOrder(81, 9);
// // makeBuyOrder(100, 10);
// makeSellOrder(81, 2);
// makeBuyOrder(100,11);
// makeBuyOrder(100, 10);
// makeSellOrder(100, 12);

// makeBuyOrder(100,30);
// makeSellOrder(100,10);


// showOrderBook();

while(true){
    string s; cin>>s;
    if(s == "stop"){
       clear();
       cout<<"Stopped"; 
       break;
    }
    else{
        if(tolower(s[0]) == 'b'){
            parse(s);
        }
        else if(tolower(s[0]) == 's'){
            parse(s);
        }
        else{
            clear();
            cout<<"Invalid input, try again\n";
        }
    }
}



return 0;
}
