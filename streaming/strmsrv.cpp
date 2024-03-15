#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService()

{
    // Update as needed
  cUser_ = NULL;

}

// To do - Complete this function
StreamService::~StreamService()
{
  for (std::vector<User*>::iterator u = users_.begin(); u != users_.end(); u++) {
    // deallocates the user
    delete (*u);
  }
  for (std::vector<Content*>::iterator c = content_.begin(); c!= content_.end(); c++) {
        // deallocates the content_
        delete(*c);
  }

}
// Complete - Do not alter
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// To do - Complete this function
void StreamService::userLogin(const std::string& uname)
{ 
  // if user is logged in
  if (cUser_ != NULL) {
    throw std::runtime_error("Please Logout First");
  }
  int index = getUserIndexByName(uname);
  // if the username DNE
  if (index == -1) {
    throw std::invalid_argument("Not a Valid User");
  }
  else {
    cUser_ = users_[index];
  }
}

// To do - Complete this function
void StreamService::userLogout()
{ 

  cUser_ = NULL;

}

// To do - Complete this function
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    // Create a vector to hold results
    std::vector<CID_T> results;
    // go through the contents in the vector
    for (std::vector<Content*>::const_iterator any = content_.begin(); any != content_.end(); ++any) {
        // if the partial string is "*" then add the content ID's
        if (partial.compare("*") == 0) {
            results.push_back((*any)->id());
        }
        // else check if contents name contains the partial string
        else {
            // find the index of partial string in content name
            size_t index = (*any)->name().find(partial);
            // if found add the content id
            if (index != std::string::npos) {
                results.push_back((*any)->id());
            }
        }
    }
    return results;
}


// Complete - Do not alter
std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

// To do - Complete this function
void StreamService::watch(CID_T contentID)
{
  throwIfNoCurrentUser();
  // if it's above the rating limit
  // if is is a valid content ID
  if (!isValidContentID(contentID)) {
    throw std::range_error("Invalid Content ID");
  }
  if (content_[contentID]->rating() > cUser_->ratingLimit) {
    throw RatingLimitError("Rating is Not Appropriate");
  }
  // adds content to the viewer's history
  else {
    content_[contentID]->addViewer(cUser_->uname);
    if (!cUser_->haveWatched(contentID)){
      cUser_->addToHistory(contentID);
    } 
  }
}

// To do - Complete this function
void StreamService::reviewShow(CID_T contentID, int numStars)
{
  throwIfNoCurrentUser();
  // if valid content ID again
  if (!isValidContentID(contentID)) {
    throw std::invalid_argument("Invalid Content ID");
  }
  // check if it's betewen 0 and 5
  if (numStars > 5 || numStars < 0) {
    throw ReviewRangeError("Review Out of Range");
  }
  content_[contentID]->review(numStars);
}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    // Change this when you are ready, but for now it is a 
    // dummy implementation
    throwIfNoCurrentUser();
    if (!isValidContentID(contentID)) {
      throw std::invalid_argument("Content ID is not valid");
    }
    // get all the users who watched the same movies
    vector<User*> u;
    for (size_t i = 0; i < users_.size(); i++) {
      // check if the user watched a movie and if they aren't the current user
      if (users_[i]->haveWatched(contentID) && users_[i] != cUser_) {
        u.push_back(users_[i]);
      }
    }
    // initialize arrays that hold
    // the amt of times, other things have been watched by u
    int size = content_.size();
    int watched[size];
    // initialize to 0;
    for (int i = 0; i < size; i++) {
      watched[i] = 0;
    }
    // for loop that loops through the users (u)
    for (size_t j = 0; j < u.size(); j++) {
      // check if the user watched the movies 
      for (int k = 0; k < size; k++) {
        if (u[j]->haveWatched(k)) {
          // increment the amount other users watched
          watched[k] = watched[k] + 1;
        }
      }
    }
    CID_T ans = -1;
    
    // loop through to find the largest
    for (int i = 0; i < size; i++) {
      // check if the user didn't watch also
      if (ans == -1) {
        if (!cUser_->haveWatched(i) && i != contentID) {
          ans = i;
        }
      }
      else {
        if (watched[i] > watched[ans] && !cUser_->haveWatched(i) && i != contentID) {
          ans = i;
        }
      }
    }
    return ans;
}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }

    // Call the display abitlity of the appropriate content object
    for (size_t i = 0; i < content_.size(); i++) {
      if (content_[i]->id() == contentID) {
        // call the display stuff
        content_[i]->display(cout);

      }
    }
}

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
