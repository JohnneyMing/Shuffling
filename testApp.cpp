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

void shuffle(unsigned int cards[], unsigned int len){

      LinkedList<int>deck1;  //To hold one deck of cards.
      LinkedList<int>deck2;  //To hold another deck of cards.

      LinkedList<int>split1; //To hold the amount of cards each time you riffle a deck of cards.
                             //My simulation is to simulate the dealer is good at shuffling cards, so each time the dealer is going
                             //to take 1 or 2 card and insert into another deck of cards.
      LinkedList<int>split2; //Only 1 card is the best condition.

      LinkedList<int>tmp;    //Useful linkedlist.
      for(int i = 0;i<len;i++)
      {
          tmp.add(i,100);
      }

      int k = 0;

      int j = 0;

      int p = 0;

      int cutPoint = rand()%5 + 24; //Generate a cutPoint when you start to cut a whole deck of cards.

      int sumnum1 = 0;

      int sumnum2 = 0;

      for(int i = 0;i<cutPoint;i++)
      {
          deck1.add(i,cards[i]);
      }

      for(int i = cutPoint;i<52;i++)
      {
          deck2.add(k,cards[i]);
          k++;
      }

      while(sumnum1 < (int)deck1.size())
      {
          //My simulation is to simulate the dealer is good at shuffling cards, so each time the dealer is going
         //to take 1 or 2 card and insert into another deck of cards.
          int num1 = rand()%2 + 1;
          split1.add(j,num1);
          j++;
          sumnum1 = sumnum1 + num1;
      }
      if(sumnum1 > (int)deck1.size())
      {
          sumnum1 - deck1.get((int)split1.size()-1);

          split1.remove((int)split1.size()-1);

          int rnd = 0;

          while(sumnum1 + rnd != (int)deck1.size())
          {
             rnd = rand()%2 + 1;
          }
           split1.add((int)split1.size(),rnd);
      }


      while(sumnum2 < (int)deck2.size())
      {
          int num2 = rand()%2 + 1;
          split2.add(p,num2);
          p++;
          sumnum2 = sumnum2 + num2;
      }
      if(sumnum2 > (int)deck2.size())
      {
          sumnum2 - deck2.get(split2.size()-1);

          split2.remove(split2.size()-1);

          int rnd = 0;

          while(sumnum2 + rnd != (int)deck2.size())
          {
             rnd = rand()%2 + 1;
          }
           split2.add(split2.size(),rnd);
      }

      //Generate a random number to decided which deck leads first.
      int leader = rand()%2;
      int index = 0;

      //deck1 leads.
      if(leader == 0)
      {
          while(deck1.size() !=0)
          {
              for(int i = 0;i<(int)split1.size();i++)
              {
                for(int j = 0;j<split1.get(i);j++)
                  {
                       int rndIndex = rand()% 2 + (index);

                       tmp.set(rndIndex,deck1.get(0));

                       deck1.remove(0);

                       index = index + 2;
                  }
              }
          }

           //Insert the cards from deck2 to the rest of slots.
           for(int i = 0;i<52;i++)
           {
           if(tmp.get(i) == 100)
           {
              tmp.set(i,deck2.get(0));

              deck2.remove(0);
           }
         }
      }

      //deck2 leads.
      else
      {
          while(deck2.size() !=0)
          {
              for(int i = 0;i<(int)split2.size();i++)
              {
                for(int j = 0;j<split2.get(i);j++)
                  {
                       int rndIndex = rand()% 2 + (index);

                       tmp.set(rndIndex,deck2.get(0));
                       deck2.remove(0);

                       index = index + 2;
                  }
              }
          }

           //Insert the cards from deck1 to the rest of slots.
           for(int i = 0;i<52;i++)
           {
            if(tmp.get(i) == 100)
            {
              tmp.set(i,deck1.get(0));
              deck1.remove(0);
            }
         }
      }

      //Add the cards from linked-list tmp back to "cards".
      for(int i = 0;i<len;i++)
      {
          cards[i] = tmp.get(i);
      }
   //finished shuffle.
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
