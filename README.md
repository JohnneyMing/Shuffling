Shuffling
=========
How many times should you shuffle a deck of cards, if you want to be certain that it is actually randomly scrambled? 
I will solve this real-life problem via simulation.


Background info

Standard decks of playing cards contain 52 cards. In order to play a game, one of the players usually attempts to randomize the order of the cards by "shuffling" them. Here are some well known types of shuffles:

"Riffle" shuffle (See: http://www.wikihow.com/Shuffle-a-Deck-of-Cards-(Riffle-Bridge) or video at http://youtu.be/o-KBNdbJOGk )
"Overhand" shuffle (See: http://youtu.be/N2xlgAFWBZM)
"Faro" or "Pharaoh" shuffle (See: http://youtu.be/ch_SMNQ-awM)
"Hindu" shuffle (See: http://youtu.be/LgVFb1U_fco)
Ideally, we would like the cards to be randomly shuffled, independently of where they started. So even if a card (say the Ace of Spades) is on top of the deck when you start, it is equally likely to be in any of the 52 possible positions at the end of the shuffle. In other words, we want the positions of the cards in the deck to be uniformly randomly distributed by the shuffle.

OpenFrameworks
--------------

This project uses openFrameworks to help visualize the results. You should expect that it will take you several days to get openFrameworks installed and working!

You can download openFrameworks from http://openframeworks.cc

For Windows, I recommend downloading the version for Visual Studio 10, and working with Visual C++ Express 10. For Mac, I recommend using XCode.

The best way to create an openFrameworks project is to start by cloning the project from github. Then go to the apps/myApps/ folder in your openFrameworks folder, and copy all the files from emptyExample project, except for src, into your repo. So you will use the src folder from github, everything else from openFrameworks.

The program ought to compile and run at this point, showing you a graphical window ... if not, it probably means that your openFrameworks library and include files are not in the right place.


Questions
=========

#### 1. Which type of shuffle did you choose? 

Riffle Shuffle.

#### 2. Explain, in English, the steps of your shuffling algorithm.

- Cut the whole deck of cards into approximately half.

- Pick 1 or 2 cards from one deck of cards and insert to a reasonable position.


#### 3. Does your algorithm reasonably simulate the actual physical process of shuffling? Explain your reasoning in a short paragraph.

Yes, it does. Check out my comments in testApp.cpp file.

#### 4. Do you use randomness in your algorithm (you probably *should*)? Explain how, and what random event it corresponds to in real life.

- Define the cutpoint when you start to cut a whole deck of cards.
- The amount of cards the dealer is going to pick when doing Waterfall method. (1 or 2).

#### 5. With your shuffle, how many times does it need to be repeated in order to give a random shuffle? Be sure to do several hundred thousand experiments.

7 times. (My simulation is based on the dealer is good at doing Riffle Shuffling, it will not give you a correct result
if you say the dealer is possible to move the top index of card to the bottom index, which means the dealer is so bad at
doing Riffle Shuffle)


