#include "content.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// To do - Complete this function
Content::Content(int id, std::string name, int nr, int ts, int rating)
{
    // initialize priv variable
    name_ = name;
    id_ = id;
    numReviews_ = nr;
    totalStars_ = ts;
    rating_ = rating;
}

// To do - Complete this function if necessary
Content::~Content()
{
    // destructor
    usersWhoViewed_.clear(); 
}

// Complete - Do not alter
void Content::display(std::ostream& ostr) const
{
    ostr << "========" << endl;
    ostr << "ID: " << id_ << endl;
    ostr << "Name: " << name_ << endl;
    ostr << "Views: " << usersWhoViewed_.size() << endl;
    ostr << "Average review: " << setprecision(1) << fixed << getStarAverage() << endl;
    
    if(rating_ >= 0 && rating_ < 5) {
        const char** ratingMapping = this->ratingStrings();
        ostr << "Rating: " << ratingMapping[rating_] << endl;
    }
}

// Complete - Do not alter
int Content::id() const
{
    return id_;
}

// Complete - Do not alter
std::string Content::name() const
{
    return name_;
}

// Complete - Do not alter
int Content::rating() const
{
    return rating_;
}

// Complete - Do not alter
void Content::review(int numStars) 
{
    numReviews_++;
    totalStars_ += numStars;
}

// Complete - Do not alter
void Content::addViewer(const std::string& username)
{
    usersWhoViewed_.push_back(username);
}

// Complete - Do not alter
const std::vector<std::string>& Content::getViewers() const
{
    return usersWhoViewed_;
}

// Complete - Do not alter
bool Content::hasViewed(const std::string& uname) const
{
    for(size_t m = 0; m < usersWhoViewed_.size(); m++){
        if(usersWhoViewed_[m] == uname){
            return true;
        }
    }
    return false;
}

// Complete - Do not alter
const char** Movie::ratingStrings() const
{
    // Global variable of movie rating mappings that you can use for 
    //  displaying Movie objects. We use "G" for rating 0 and 1.
    static const char* movierating[] = {"G", "G", "PG", "PG-13", "R"};
    return movierating;
}

// Complete - Do not alter
const char** Series::ratingStrings() const
{
    // DEFAULT TO THE SERIES / TV Rating Mappings
    
    // Global variable of series rating mapping (0 = "TVY", 1 = "TVG", etc.)
    static const char* tvrating[] = {"TVY", "TVG", "TVPG", "TV14", "TVMA"};
    return tvrating;
}

// To do - Complete this function
double Content::getStarAverage() const 
{
    if (numReviews_ == 0) {
      return 0.0;
    }
    // static_cast won't work here because it completes the double conversion
    // after the computation and so we were getting 3.0 instead of 3.7 or something
    return (double)totalStars_/numReviews_;
}

// To do - Complete this function 
int Series::numEpisodes() const
{
    // Should return the number of episodes in the series
    return numEpisodes_;

}

// Complete the other derived class member constructors and member functions
Series::Series(int id, std::string name, int nr, int ts, int rating,
 int numEpisodes)
 : Content(id, name, nr, ts, rating) {
    numEpisodes_ = numEpisodes;
}

Movie::Movie(int id, std::string name, int nr, int ts, int rating)
 : Content(id, name, nr, ts, rating) {

}

void Series::display(std::ostream& ostr) const{
  Content::display(ostr);
  ostr << "Episodes: " << numEpisodes_ << endl;
}
Series::~Series() {
}
Movie::~Movie() {
}
