#include "Asymmetry.h"

using namespace std;

Asymmetry::Asymmetry(const char * inputDir){
  _inputDir = inputDir;
  void *dir = gSystem->OpenDirectory(_inputDir);
  char * subdir;
  while((subdir = const_cast<char *>(gSystem->GetDirEntry(dir)))){
    // Save full path of subdirectory
    char * subdirpath = Form("%s/%s",inputDir,subdir);

    // Split subdirectory by keyword "_"
    charVect cvect = splitString(subdir,"_");
    
    // Test if subdirectory contains keyword "bin"
    if(charVectorContains(cvect, _keyword)==false)
      continue;
    
    // Create new asymmetry bin
    // 1. Set subdirpath (starting from root)
    // 2. Find method type from prefix
    // 3. Set bin specific variables (binVar, min, max)
    asymBin ab;
    ab.dirpath = subdirpath;
    if(charVectorContains(cvect,"sideband")==true)
      {
	ab.method = sideband;
      }
    else if(charVectorContains(cvect,"splot")==true)
      {
	ab.method = splot;
      }
    else if(charVectorContains(cvect,"cows")==true)
      {
	ab.method = cows;
      }
    else
      continue;

    if(setBin(cvect,ab)==-1)
      continue;
    else
      _asymBins.push_back(ab);
  }
}

// Splits string/char based on delimeter
// Example:
// splitString("how_are_you","_") would return
// std::vector<char *> = {"how","are","you"}
charVect Asymmetry::splitString(char * string, const char * delimiter){
  charVect splits;
  std::istringstream iss(std::string(string));
  std::string token;
  while (std::getline(iss, token, delimiter))
    {
      splits.push_back(token.c_str());
    }
  return splits;
}

// Checks if char * vector contains keyword
bool Asymmetry::stringVectorContains(charVect cvect, const char* keyword){
  for(char * c: cvect){
    int x = std::string(c).compare(std::string(keyword));
    if(x==0)
      return true; // Match!
  }
  return false; // No matches found
}

// Returns std::string exactly "skip" places after the keyword
// Example
// charVectorAfter({"alpha","beta","gamma","omega"} , "beta" , 2)
// returns "omega"
std::string Asymmetry::charVectorFind(charVect cvect, const char* keyword, int skip){
  int i = 0;
  for(char * c : cvect){
    int x = std::string(c).compare(std::string(keyword));
    if(x==0){
      if(i+skip>cvect.size()-1 || i+skip < 0){
	cout << "ERROR in Asymmetry::charVectorFind --- Out of bounds exception with respect to skip variable. The keyword index PLUS skip variable must be within [0," << cvect.size()-1 << "], however index+skip = " << i << "+" << skip << " = " << i+skip << ". Returning empty std::string" << endl;
	return std::string("");
      }
      return std::string(cvect.at(i+skip));
    }
    else
      i++;
  }
  cout << "WARNING in Asymmetry::charVectorFind --- No keyword found. Returning empty string" << endl;
  return std::string("");
}

// Returns index of keyword in charVect
int Asymmetry::charVectorIndex(charVect cvect, const char* keyword){
  int i = 0;
  for(char * c : cvect){
    int x = std::string(c).compare(std::string(keyword));
    if(x==0)
      return i;
    else
      i++;
  }
  cout << "WARNING in Asymmetry::charVectorIndex --- Cannot find keyword. Return -1" << endl;
  return -1;
}

// Set the bin related variables based on cvect info
bool Asymmetry::setBin(charVect cvect, asymBin & ab){
  // Get index of keyword in char array
  int idx = charVectorIndex(cvect,_keyword);
  // Get size of char array
  int L = (int)cvect.size();
  // Extract number of binnings
  if((L-idx)%3!=0){
    cout << "ERROR in Asymmetry::setBin --- After the key word, there must be a 'N' distinct terms separate by '_' , where 'N' is divisible by 3. This is not the case. The required format from the Binning code should be 'method_bin_binVar1_min1_max1_binVar2_min2_max2... Returning -1" << endl;
    return -1;
  }
  int nBins = (L-idx)/3;
  // Set bin names, bin mins, bin maxs
  for(int b = 0 ; b < nBins; b++){
    ab.binVar.push_back(charVectorFind(cvect,_keyword,b*3).c_str());
    ab.binMin.push_back(std::stof(charVectorFind(cvect,_keyword,b*3+1)));
    ab.binMax.push_back(std::stof(charVectorFind(cvect,_keyword,b*3+2)));
  }
  return 0;
}
