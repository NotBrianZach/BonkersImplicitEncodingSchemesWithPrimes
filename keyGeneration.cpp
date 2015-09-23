#include <ctype.h>
#include <sys/stat.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <iterator>
#include <gmpxx.h>
#include <stdarg.h>

//HELPER FUNCTIONS AND CLASSES, main logic further down

//IMPORTANT GLOBALS
struct Globals{
  //this sets the range over which exponents
  static const int exponentMin = 6;
  static const int exponentMax = 9;

  //need to build a "lookup array" of "keys"-
  //numbers that we know all the factors to
  //then when we need to encode a number,
  //we take the difference between the
  //closest number in this array and that number, store that
  //and store the representation of the number in this array
  //which is stored in the array next to the number,
  //except in our special programmatic representation
  //where every set number of bits, you raise the base
  //to the next available prime

  static constexpr unsigned int primes[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
  static const unsigned int numAvailablePrimes = sizeof(primes) / sizeof(primes[0]);
  //max value = 315 bits, 7.6E62
  //our representation will be 32 bits for each key.

  //std::vector<unsigned int> primes (initializerArray, initializerArray + numPrimes );
  //deprecated: + sizeof(initializerArray) / sizeof(initializerArray[0])

  //this along with the actual length of the compact representation,
  //will help us find the appropriate key to subtract from the number/data to be encoded.
  static const unsigned int howManyOfFirstNBitsStoredInKey = 32;
  static constexpr unsigned int primeRepresentationLength = (exponentMax-exponentMin)*numAvailablePrimes;
} globals;
//CARTESIAN PRODUCT CODE
// we want to take the cartesian prouct of the vectors of exponents.
//http://stackoverflow.com/questions/5279051/how-can-i-create-cartesian-product-of-vector-of-vectors


// Types to hold vector-of-unsigned ints (Vi) and vector-of-vector-of-unsigned ints (Vvi)
typedef std::vector<unsigned int> Vi;
typedef std::vector<Vi> Vvi;
// Seems like you'd want a vector of iterators
// which iterate over your individual vector<unsigned int>s.
struct Digits {
  Vi::const_iterator begin;
  Vi::const_iterator end;
  Vi::const_iterator me;
};

int intPow (int x, int p) {
  int i = 1;
  for (int j = 1; j <= p; j++)  i *= x;
  return i;

}

typedef std::vector<Digits> Vd;

//populate the unsigned input data set
Vvi build_input() {
  //here we populate the lower and upper bounds for the exponents of the primes
  Vvi vvi;
  for(unsigned int i = 0; i < globals.numAvailablePrimes; i++) {//this index determines which prime
    // {2=index 0,3 = index 1, 5= index 2, 7 = index 3, 11 = index 4, 13 = index 5}
    Vi vi;
    for(unsigned int j = globals.exponentMin; j < globals.exponentMax+1; j++) {//this index generates the exponents
      //so we will produce combination like
      //2^6 * 3^6 * 5^6....
      //2^7 * 3^6 * 5^6...
      //2^7 * 3^7 * 5^6...
      vi.push_back(j);
    }
    vvi.push_back(vi);
  }
  return vvi;
}

// just for the sample -- prunsigned int the data sets
std::ostream&
  operator<<(std::ostream& os, const Vi& vi)
{
  os << "(";
  std::copy(vi.begin(), vi.end(), std::ostream_iterator<unsigned int>(os, ", "));
  os << ")";
  return os;
}
std::ostream&
  operator<<(std::ostream& os, const Vvi& vvi)
{
  os << "(\n";
  for(Vvi::const_iterator it = vvi.begin();
      it != vvi.end();
      it++) {
    os << "  " << *it << "\n";
  }
  os << ")";
  return os;
}


typedef std::vector<Digits> vd;
void cart_product(Vvi& in,Vvi& out){
  Vd vd;
  // Start all of the iterators at the beginning.
  for(Vvi::const_iterator it = in.begin(); it != in.end(); ++it) {
    Digits d = {(*it).begin(), (*it).end(), (*it).begin()};
    vd.push_back(d);
  }

  while(1) {
    // Construct your first product vector by pulling
    // out the element of each vector via the iterator.
    Vi result;
    for(Vd::const_iterator it = vd.begin();
        it != vd.end();
        it++) {
      result.push_back(*(it->me));
    }
    out.push_back(result);

    // Increment the rightmost one, and repeat.

    // When you reach the end, reset that one to the beginning and
    // increment the next-to-last one. You can get the "next-to-last"
    // iterator by pulling it out of the neighboring element in your
    // vector of iterators.
    for(Vd::iterator it = vd.begin(); ; ) {
      // okay, I started at the left instead. sue me
      ++(it->me);
      if(it->me == it->end) {
        if(it+1 == vd.end()) {
          // I'm the last digit, and I'm about to roll
          return;
        } else {
          // cascade
          it->me = it->begin;
          ++it;
        }
      } else {
        // normal
        break;
      }
    }
  }
}

struct KeyObject {
  //this is in base 2.
    unsigned int unencodedLength;
    std::bitset<globals.howManyOfFirstNBitsStoredInKey> firstNDigits;
    std::bitset<globals.howManyOfFirstNBitsStoredInKey> primeRepresentation;
};

//FILE INPUT READING, getting data to encode
//TODO MOVE THIS UNSIGNED INTO A DIFFERENT FILE
void quit() /* write error message and quit */{
  std::cout <<  "memory exhausted\n" << std::endl;
  exit(1);
}

char* getInputFromFile () {
  unsigned int max = 20;
  char* name = (char*)malloc(max); /* allocate buffer */
  if (name == 0) quit();

  std::cout <<"Enter a file name: "<< std::endl;

  while (1) { /* skip leading whitespace */
    unsigned int c = getchar();
    if (c == EOF) break; /* end of file */
    if (!isspace(c)) {
      ungetc(c, stdin);
      break;
    }
  }

  unsigned int i = 0;
  while (1) {
    unsigned int c = getchar();
    if (isspace(c) || c == EOF){ /* at end, add terminating zero */
      name[i] = 0;
      break;
    }
    name[i] = c;
    if (i == max - 1) { /* buffer full */
      max = max + max;
      name = (char*)realloc(name, max); /* get a new and larger buffer */
      if (name == 0) quit();
    }
    i++;
  }

  std::cout << "The filename is " << name << std::endl;
  free(name); /* release memory */
  return name;
}
//MAIN LOGIC BEGINS
int main() {

  //first unsigned int=total length of the actual representation, char = first 8 digits of the actual representation,
  //each KeyObject has these properties:
  //unsigned int unencodedLength; the total length of the unencoded number
  //unsigned int firstNDigits; the first 32 digits of the unencoded number
  //unsigned int primeRepresentation; the encoded representation of the number/key
  //std::vector<KeyObject> keys;
  //for(unsigned int i=0; i < 10000; i++){
  //    keys.push_back(KeyObject());
  //}

  ////unsigned int initialLowerBoundIndex = 0;
  ////recursiveFillKeys(lowerBounds,upperBounds,initialLowerBoundIndex,keys,primes);

  ////simple examle of generating the keys structure. (these are the numbers we will reduce the file to,
  ////we store them in our ultra compact representation with some identifying info)
  ////for (unsigned int i=6; i<10; i++){
  ////  for (unsigned int j=6; j<10; j++){
  ////    keys[(h-6)*16 + (i-6)*4 + (j-6)] = unsigned integerExponent(prime[0],i)*unsigned integerExponent(prime[1],j)....
  ////  }
  ////}

  //Vvi input(build_input());
  //std::cout << input << "\n";

  //Vvi output;
  //cart_product(input, output);
  //std::cout << output << "\n";

  ////sample input/output
  ////input
  ////  (
  ////   (0, 1, 2, )
  ////   (10, 11, 12, )
  ////   (20, 21, 22, )
  ////   )
  ////  output
  ////  (
  ////   (0, 10, 20, )
  ////   (1, 10, 20, )
  ////   (2, 10, 20, )
  ////   (0, 11, 20, )
  ////   (1, 11, 20, )
  ////   (2, 11, 20, )
  ////   (0, 12, 20, )
  ////   (1, 12, 20, )
  ////   (2, 12, 20, )
  ////   (0, 10, 21, )
  ////   (1, 10, 21, )
  ////   (2, 10, 21, )
  ////   (0, 11, 21, )
  ////   (1, 11, 21, )
  ////   (2, 11, 21, )
  ////   (0, 12, 21, )
  ////   (1, 12, 21, )
  ////   (2, 12, 21, )
  ////   (0, 10, 22, )
  ////   (1, 10, 22, )
  ////   (2, 10, 22, )
  ////   (0, 11, 22, )
  ////   (1, 11, 22, )
  ////   (2, 11, 22, )
  ////   (0, 12, 22, )
  ////   (1, 12, 22, )
  ////   (2, 12, 22, ))
  //unsigned int counter = 0;
  //for(Vvi::iterator it = output.begin(); ; ) {
  //  //keyExponentValues gives us a vector with all the exponents we need to use
  //  //to create a key
  //  //here we calculate the number represented by the exponents

  //  mpz_t n;
  //  mpz_init(n);
  //  mpz_set(n,1);
  //  for (Vi::iterator keyExponentValues = it->begin(); ; ){
  //    mpz_t k;
  //    mpz_init(k);
  //    mpz_set(k,globals.primes[counter]);
  //    mpz_mul(n,n,k);
  //    n *= k;
  //    mpz_clear (k);
  //    //mpz_sizeinbase (mpz_t op, int base)
  //    //mpz_sizeinbase (mpz_t op, int base)
  //    //base can only be up to size 62 max!
  //    //here we store the length of the number represented by the exponents
  //    keys[counter].unencodedLength = 0;

  //    //here we store the first n digits of the number represented by the exponents
  //    keys[counter].firstNDigits = 0;
  //    mpz_clear (n);

  //    //this simulates log base 2
  //    unsigned int bitsToStoreExponent= 0;
  //    unsigned int exponentRange = globals.exponentMax - globals.exponentMin;
  //    while (exponentRange >>= 1) ++bitsToStoreExponent;
  //    //IMPORTANT ENCODING LOCATED RIGHT HERE IN THIS COMMENT!!!
  //    //00 = 6th power, 01=7th, 10=8th, 11=9th,
  //    //first 2 bits are for 2, next 2 are for 3, next 2 are for 5, etc. etc.
  //    unsigned int exponentCounter = 0;
  //    for(Vi::iterator keyExponentValues = it->begin(); ; ) {
  //      for (int j=globals.exponentMin; j<globals.exponentMax; j++){
  //        for (int k=0;k<bitsToStoreExponent;k++){
  //          //set i'th prime number bits to appropiate value between 0 and 2^k for exponent value j
  //          //globals.exponentMin - j want to turn this into binary representation, get appropriate number for ith place
  //          if (k==j){
  //            //keys[counter].primeRepresentation();
  //            //(globals.exponentMin - j) might be useful
  //            if (*keyExponentValues % intPow(2,k)==0){//base 10 number convert to base two, get ith digit.
  //              keys[counter].primeRepresentation.set(globals.howManyOfFirstNBitsStoredInKey
  //                                                    - exponentCounter*bitsToStoreExponent,1);
  //            }
  //            else{
  //              keys[counter].primeRepresentation.set(globals.howManyOfFirstNBitsStoredInKey
  //                                                    - exponentCounter*bitsToStoreExponent,0);
  //            }
  //          }
  //        }
  //      }
  //      //illustrative but less general example
  //      //switch(keyExponentValues->me[i]){
  //      //  case 6:
  //      //    keys[counter].primeRepresentation();
  //      //    keys[counter].primeRepresentation.set[32-i*2,0];
  //      //    keys[counter].primeRepresentation.set[31-i*2,0];
  //      //    break;
  //      //  case 7:
  //      //    keys[counter].primeRepresentation();
  //      //    keys[counter].primeRepresentation.set[32,0];
  //      //    keys[counter].primeRepresentation.set[31,1];
  //      //    break;
  //      //  case 8:
  //      //    keys[counter].primeRepresentation();
  //      //    keys[counter].primeRepresentation.set[32,1];
  //      //    keys[counter].primeRepresentation.set[31,0];
  //      //    break;
  //      //  case 9:
  //      //    keys[counter].primeRepresentation();
  //      //    keys[counter].primeRepresentation.set[32,1];
  //      //    keys[counter].primeRepresentation.set[31,1];
  //      //    break;
  //      //}
  //      exponentCounter++;
  //      keyExponentValues++;
  //    }
  //  }
  //  counter++;
  //  it++;
  //}



  //new plan:
  //1. strip leading zeroes from beginning of file, record number in encoding file.
  //3. get size of file, number of primes used = log base 2 of size of file in bits, round down
  //3. calculate minimum exponent to use
  //4. subtract minimum exponent from file binary value, feed remainder back into algorithm
  //5. iterate over minimum over a range of exponents, until you find one with a minimum number of similar bits.

  //crazy plan
  //6. flip numbers over every time: we eat all of the lower digits, convert them to 0's,
  //then encode those with a number and start the end of the next number at the beginning
  //of the previous number

  //output (all serialzed with boost):
  //1. unsigned int, derived from the file size - leading zeroes, this tells us how many primes we will be using
  //2. unsigned int, minimum exponent
  //3. series of bitsets, with signed ints in between indicating the number of primes in use
  //4. if there is a negative remainder, a - int serialized before the last bitset, indicates to subtract from previous values, with whatever number of primes as indicated by the absolute value of the number

  //crazy output:
  //0. number that indicates how many times to recursively encode/decode the file, INSANITY!
  //1. unsigned int, derived from the file size - leading zeroes, this tells us how many primes we will be using
  //2. unsigned int, minimum exponent
  //3. number of leading zeros
  //4. signed int indicating the number of primes to use
  //5. unsigned int, minimum exponent
  //6. bitset, encoding exponent values over a set range
  //7. if there is a negative remainder, a - int serialized before the last bitset, indicates to subtract from previous values, with whatever number of primes as indicated by the absolute value of the number

  //crazy decode:
  //read unsigned int, use a subset of primes that is held in common between you and encoder
  //read minimum exponent
  //write leading zeroes to a file
  //if first number is negative, we fail hard.
  //calculate binary value from bitset


  //char* input = getInputFromFile();
  char fileNameToEncode[11] = "./inNumber";
  char encodedFileName[23] = "./compactRepresenation";

  //get file size
  struct stat st;
  stat(fileNameToEncode, &st);
  unsigned int fileSizeToEncode = st.st_size;

  //this simulates log base 2, must be done with unsigned int
  unsigned int numPrimesBasedOnFileSize = 0;
  unsigned int takingLogOfFileSizeToEncode= fileSizeToEncode;
  while (takingLogOfFileSizeToEncode >>= 1) ++numPrimesBasedOnFileSize;

  //this simulates log base 2, must be done with unsigned int
  unsigned int bitsToStoreExponent = 0;
  unsigned int exponentRange = globals.exponentMax - globals.exponentMin;
  while (exponentRange >>= 1) ++bitsToStoreExponent;

  //Compute the minimum exponent
  //multiply all the n primes we need together, then take exponent (1/n), (geometric avg)
  //then divide geometric avg by number of primes to get the minimum exponent for each primes
  //to produce a power of 2 the same magnitude as the magnitude of the number represeneted by the file
  //read from left to right without leading zeros.
  unsigned int primeProduct = 1;
  for (int primeCounter=0;primeCounter<numPrimesBasedOnFileSize;i++)
    primeProduct *= primes[primeCounter];
  double primeGeometricMean = pow(primeProduct,1/numPrimesBasedOnFileSize);
  //file size to encode /log base 2 of geometric mean
  //mean^x=2^10000000 example problem,take log2 of both side divide by mean 
  unsigned int minimumExponent = std::ceil(fileSizeToEncode / primeGeometricMean)/numPrimesBasedOnFileSize;
  //HERE IS WHERE WE READ THE FILE IN THAT WE WILL ENCODE
  FILE *fileToEncode = fopen(fileNameToEncode,"r");
  FILE *encodedFile = fopen(encodedFileName,"r");

  //first we read the first n bytes, cut out any leading zeros, and store the number of zeros in serialzed format
  //seek back to the beginning of the file
  fseek(fileToEncode, 0L, SEEK_SET);
  //read bits, count number of zeros, leave file pointer on first bit with value 1
  char firstByteOfFile = '0';
  unsigned int countLeadingZeroes = 0;
  streampos pos;
  ifstream infile;
  infile.open(fileToEncode, ios::binary | ios::in);
  //infile.seekp(243, ios::beg); // move 243 bytes into the file
  infile.read(&x, sizeof(char));
  while (firstByteOfFile == '0'){
    infile.seekp(1, ios::beg); // move 243 bytes into the file

  }
  pos = infile.tellg();
  cout << "The file pointer is now at location " << pos << endl;
  infile.seekp(0,ios::end); // seek to the end of the file
  infile.seekp(-10, ios::cur); // back up 10 bytes
  infile.close(); 

  //store number of zeros in encodedFile

  //store the rest of bits of fileToEncode in a binary mpz number
  mp_size_t mpn_get_str (unsigned char *STR, int BASE,
                         mp_limb_t *S1P, mp_size_t S1N)
  //  mpz_t k;
  //  mpz_init(k);
  //  mpz_set(k,fileToEncode);

  //multiply the minimum exponent primes together that we computed earlier

  //subtract mpz number
  //  mpz_t n;
  //  mpz_init(n);
  //  mpz_set(n,1);


  //  mpz_mul(n,n,k);
  //  mpz_clear (k);


  //seek to the end of the file
  fseek(fileToEncode, 0L, SEEK_END);
  //readfileinreverse(fileToEncode);


  //unsigned int i = 0;
  //while (input[i] != '\0'){
  //}
  return 0;
}



















//EVERYTHIBNG BELOW IS DEPRECATED

//void recursiveFillKeys(vector<unsigned int>& lowerBounds, const vector<unsigned int>& upperBounds, unsigned int currentIndex,  vector<KeyObject>& keys, vector<unsigned int>& primes){
//  if (currentIndex == indexes.size()) {
//    // This is where the real logic goes.
//    // indexes[i] contain the value of the i-th index.
//    for (unsigned int i=lowerBounds[pos]; i<upperBounds[pos]; i++){
//      //keys[(h-6)*16 + (i-6)*4 + (j-6)] = unsigned integerExponent(prime[0],i)*unsigned integerExponent(prime[1],j)....
//      keys[(h-6)*16 + (i-6)*4 + (j-6)] = unsigned integerExponent(prime[0],i)*unsigned integerExponent(prime[1],j);
//      //primes[j];
//
//
//    }
//  } else {
//    for (indexes[pos] = 0 ; indexes[pos] != endPerIndex[pos] ; indexes[pos]++) {
//      // Recurse for the next level
//      recursiveLoops(indexes, endPerIndex, pos+1, lowerBounds, upperBounds);
//    }
//  }
//
//}

//GMP TIPS

//Small Operands
//On small operands, the time for function call overheads and memory
//                    allocation can be significant in comparison to actual calculation.
//                    This is unavoidable in a general purpose variable precision
//               library, although GMP attempts to be as efficient as it can on
//                    both large and small operands.
//
//               Static Linking
//               On some CPUs, in particular the x86s, the static `libgmp.a' should
//     be used for maximum speed, since the PIC code in the shared
//     `libgmp.so' will have a small overhead on each function call and
//                    global data address.  For many programs this will be
//               insignificant, but for long calculations there's a gain to be had.
//
//Initializing and Clearing
//     Avoid excessive initializing and clearing of variables, since this
//     can be quite time consuming, especially in comparison to otherwise
//     fast operations like addition.
//
//     A language interpreter might want to keep a free list or stack of
//     initialized variables ready for use.  It should be possible to
//     integrate something like that with a garbage collector too.
//
//Reallocations
//     An `mpz_t' or `mpq_t' variable used to hold successively increasing
//     values will have its memory repeatedly `realloc'ed, which could be
//               quite slow or could fragment memory, depending on the C library.
//                    If an application can estimate the final size then `mpz_init2' or
//     `mpz_realloc2' can be called to allocate the necessary space from
//               the beginning (*note Initializing Integers::).
//
//                      It doesn't matter if a size set with `mpz_init2' or `mpz_realloc2'
//     is too small, since all functions will do a further reallocation
//     if necessary.  Badly overestimating memory required will waste
//     space though.
//`
//3.12 Debugging
//==============
//
//               Stack Overflow
//               Depending on the system, a segmentation violation or bus error
//                    might be the only indication of stack overflow.  See
//                    `--enable-alloca' choices in *note Build Options::, for how to
//     address this.
//
//     In new enough versions of GCC, `-fstack-check' may be able to
//                    ensure an overflow is recognised by the system before too much
//               damage is done, or `-fstack-limit-symbol' or
//     `-fstack-limit-register' may be able to add checking if the system
//                    itself doesn't do any (*note Options for Code Generation:
//     (gcc)Code Gen Options.).  These options must be added to the
//     `CFLAGS' used in the GMP build (*note Build Options::), adding
//            them just to an application will have no effect.  Note also
//            they're a slowdown, adding overhead to each function call and each
//     stack allocation.
//
//Heap Problems
//     The most likely cause of application problems with GMP is heap
//     corruption.  Failing to `init' GMP variables will have
//       unpredictable effects, and corruption arising elsewhere in a
//            program may well affect GMP.  Initializing GMP variables more than
//            once or failing to clear them will cause memory leaks.
//
//            In all such cases a `malloc' debugger is recommended.  On a GNU or
//     BSD system the standard C library `malloc' has some diagnostic
//       facilities, see *note Allocation Debugging: (libc)Allocation
//               Debugging, or `man 3 malloc'.  Other possibilities, in no
//     particular order, include
//
//          `http://www.inf.ethz.ch/personal/biere/projects/ccmalloc/'
//       `http://dmalloc.com/'
//     `http://www.perens.com/FreeSoftware/'  (electric fence)
//     `http://packages.debian.org/stable/devel/fda'
//     `http://www.gnupdate.org/components/leakbug/'
//     `http://people.redhat.com/~otaylor/memprof/'
//     `http://www.cbmamiga.demon.co.uk/mpatrol/'
//
//          The GMP default allocation routines in `memory.c' also have a
//     simple sentinel scheme which can be enabled with `#define DEBUG'
//                    in that file.  This is mainly designed for detecting buffer
//               overruns during GMP development, but might find other uses.
