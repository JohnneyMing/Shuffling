#include "testApp.h"
#include "ArrayQueue.h"
#include "LinkedList.h"


//How many experiments should we do between each re-draw of the screen?
// Bigger numbers make the program much faster
#define EXPS_PER_UPDATE 10
//How many times should I call shuffle before measuring the result?
#define SHUFFLES_PER_EXP 7

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(255);

    //old OF default is 96 - but this results in fonts looking larger than in other programs.
        ofTrueTypeFont::setGlobalDpi(72);

    arial24.loadFont("arial.ttf",24);

    for(int i=0;i<52*52;i++){
        shuffleCounts[i] = 0;
        randCounts[i] = 0;
    }
    experimentsDone = 0;

    std::srand ( unsigned ( std::time(0) ) );
}

//--------------------------------------------------------------
void testApp::update(){
    for(int i=0; i<EXPS_PER_UPDATE; i++){
        doShuffleExperiment(SHUFFLES_PER_EXP);
        doRandExperiment();
        experimentsDone += 1;
    }
}

//-----------
//Print out a matrix so that high numbers are white, low numbers are black
//-----
void testApp::drawMatrix(float x, float y, float width, float height, double matrix[], double min, double max){
    float sqWidth = width/52.0;
    float sqHeight = height/52.0;

    for(int i=0;i<52;i++){
        for(int j=0;j<52;j++){
            //j corresponds to x, i corresponds to y
            double intensity = (matrix[j+i*52]-min)/(max-min);
            if(intensity < 0) intensity = 0;
            if(intensity > 1) intensity = 1;
            ofSetColor((int)(255*intensity));
            ofFill();
            ofRect(x+j*sqWidth, y+i*sqHeight, sqWidth, sqHeight);
        }
    }
}

//--------------------------------------------------------------
//Draw should always be as fast as possible, so any heavy computations
// should be done in update.
//-----
void testApp::draw(){
    //Draw the two matrices
    drawMatrix(5,5+24+5+24+5,260,260,randCounts, 0, experimentsDone/26);
    drawMatrix(260+10,5+24+5+24+5,260,260,shuffleCounts,0,experimentsDone/26);

    //Draw info about how long we have been running
    //Draw text in black
    ofSetColor(0);
    ostringstream stringBuilder;
    stringBuilder << experimentsDone << " experiments done" << flush;
    double strWidth = arial24.stringWidth(stringBuilder.str());
    double strX = (ofGetWidth() - strWidth)/2;
    arial24.drawString(stringBuilder.str(), strX, 260+5+24+5+24+24+5);

    //Clear out the string buffer for re-use
    stringBuilder.clear();
    stringBuilder.str("");

    //Calculate the p value for true random, and print it
    stringBuilder << "p < " << pValue(randCounts) << flush;
    strWidth = arial24.stringWidth(stringBuilder.str());
    strX = 5 + (260 - strWidth)/2;
    arial24.drawString(stringBuilder.str(), strX, 130+5+24+5+24+24+5);

    //Clear out the string buffer for re-use
    stringBuilder.clear();
    stringBuilder.str("");

    //Calculate the p value for shuffle, and print it
    stringBuilder << "p < " << pValue(shuffleCounts) << flush;
    strWidth = arial24.stringWidth(stringBuilder.str());
    strX = 10 + 260 + (260 - strWidth)/2;
    arial24.drawString(stringBuilder.str(), strX, 130+5+24+5+24+24+5);

    //Clear out the string buffer for re-use
    stringBuilder.clear();
    stringBuilder.str("");

    //Calculate the p value for shuffle, and print it
    stringBuilder << "If shuffle is truly random, what is chance\nof this outcome? (small is bad)" << flush;
    strWidth = arial24.stringWidth(stringBuilder.str());
    strX = (ofGetWidth() - strWidth)/2;
    arial24.drawString(stringBuilder.str(), strX, 24);
}

/*
 * Use this as a comparison for your shuffle algorithm. This
 * is an implementation of the Fisher-Yates Shuffle
 * (http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle )
 * which is generally considered to be the right way to generate
 * a uniformly random permuation.
 */
void randomize(unsigned int cards[], unsigned int len){
    for(int i=1; i<len; i++){
        //We are going to swap item len-i with a random
        // item that comes before it (or itself)
        unsigned int t = cards[len-i];
        unsigned int which = rand()%(len-i+1);

        cards[len-i] = cards[which];
        cards[which] = t;
    }
}

void testApp::doRandExperiment(){
    //Initialize a deck of cards
    static unsigned int cards[52];
    for(int i=0;i<52;i++){
        cards[i] = i;
    }

    //randomly scramble it using Fisher-Yates shuffle
    randomize(cards,52);

    for(int i=0;i<52;i++){
        randCounts[i*52 + cards[i]] += 1.0;
    }
}


//"Riffle shuffle" style.
//Author: MINGWEI ZHONG
void shuffle(unsigned int cards[], unsigned int len){
    
    //When you start doing riffle shuffle, first of all, you need to cut the whole deck of cards into approximately half.
    //Here I create two linked-list to hold these two decks of cards.
      LinkedList<int>deck1;  
      LinkedList<int>deck2;


      //Useful temporary linked-list.
      LinkedList<int>tmp;
      
      //Set the value of temporary linked-list to be 100.
      for(int i = 0;i<len;i++)
      {
          tmp.add(i,100);
      }

      int k = 0;
      int j = 0;
      int p = 0;

      
      //Generate a cutpoint from 22 to 29, I think it is a reasonable number.
      //My simulation is to simulate a person is good at shuffling.
      int cutPoint = rand() % 8 + 22;

      for(int i = 0;i<cutPoint;i++)
      {
          deck1.add(i,cards[i]);
      }

      for(int i = cutPoint;i<52;i++)
      {
          deck2.add(k,cards[i]);
          k++;
      }

      int index = 0;

      if((int)deck1.size() <= (int)deck2.size())
      {
          while(deck1.size() !=0)
          {
              for(int i = 0;i<(int)deck1.size();i++)
              {
                      //Here is the important part, in order to simulate
                      //a real shuffling process, the cards locate at top, middle, and bottom 
                      //location have high probability to be inserted into top, middle, and bottom
                      //location after shuffling.
                      
                      //Which means, the cards locate at top position will never be inserted to the
                      //bottom location if you are doing real shuffling.
                      //That's why I increase the "index" value as 2 so the cards locat at different 
                      //positions can be reasonably distributed into a new position.
                      
                      //Overall, I am simulating a dealer picks 1 or 2 cards from one of these
                      //two decks of cards and insert them to reasonable positions which the cards ought to be.
                
                       int rndIndex = rand()% 2 + (index);

                       tmp.set(rndIndex,deck1.get(0));

                       deck1.remove(0);

                       index = index + 2;
              }
          }

           for(int i = 0;i<len;i++)
           {
               //Insert the cards from another deck of cards
               //into the rest of positions.
               if(tmp.get(i) == 100)
               {
                 tmp.set(i,deck2.get(0));
                 deck2.remove(0);
               }
          }
          for(int i = 0;i<(int)tmp.size();i++)
          {
              //Update the "cards" after shuffling.
              cards[i] = tmp.get(i);
          }
      }

      //If the size of deck2 is larger than deck1.
      else
      {
          while(deck2.size() !=0)
          {
              for(int i = 0;i<(int)deck2.size();i++)
              {
                       int rndIndex = rand()% 2 + (index);

                       tmp.set(rndIndex,deck2.get(0));

                       deck2.remove(0);

                       index = index + 2;
              }
           }

           for(int i = 0;i<len;i++)
           {
             if(tmp.get(i) == 100)
                {
                  tmp.set(i,deck1.get(0));
                  deck1.remove(0);
                }
           }
           for(int i = 0;i<(int)tmp.size();i++)
           {
                 cards[i] = tmp.get(i);
           }
      }
  //Finished shuffling.
}

void testApp::doShuffleExperiment(int numShuffles){
    //Initialize a deck of cards
    static unsigned int cards[52];
    for(int i=0;i<52;i++){
        cards[i] = i;
    }

    //randomly scramble it using shuffle
    for(int i=0; i<SHUFFLES_PER_EXP;i++){
        shuffle(cards,52);
    }

    for(int i=0;i<52;i++){
        shuffleCounts[i*52 + cards[i]] += 1.0;
    }
}

std::string testApp::pValue(double matrix[]){
    //This implements the test of indepence, viewing our matrix
    // as a contingency table. See http://en.wikipedia.org/wiki/Pearson%27s_chi-squared_test
    //Note, however, that we are only testing one row of the matrix, which means
    // we are only testing one card ... not the whole deck.
    // To pass this test your algorithm only has to make sure the first
    // card is randomly moved. In fact, it isn't possible to use
    // a statistical test to prove a shuffle algorithm is truly random.
    // Instead, you would have to really prove it using probability theory.
    // This is just a quick and dirty test that is helpful for debugging.

    double chiSq = 0.0;
    double expected = ((double)experimentsDone)/52;
    for(int i=0;i<52;i++){
        double observed = matrix[i];
        chiSq += (observed-expected)*(observed-expected);
    }
    chiSq = chiSq/expected;

    //Critical values for 51 degrees of freedom
    double chi[] = {0.0,42.36,47.84,52.92,59.25,64.3,68.67,77.39,80.75};
    std::string p[] = {"100%","80%","60%","40%","20%","10%","5%","1%","0.5%"};

    int index = 0;
    while(index < 8){
        if(chiSq >= chi[index+1]){
            index++;
        } else {
            break;
        }
    }
    return p[index];
}
