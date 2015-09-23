from bitstring import BitArray, BitStream
import math
import itertools
import random
#naive implementation doesn't work, may be able to get to work
#by narrowly targeting a small range of big numbers.

#new strategy, partial factor into powers of 2 and 3,
#subtract from number, then partial factor next number

#to decode multiply together then add successive terms

#growth 2^k vs. n choose k
#k digit number has 2^k possible values
#each exponent bit can be combined with any of the other
#exponent bits

def numBinaryDigitsEqual0(numberToEncode):
##start with 4 or 3 bits for base4, can represent up to 4 or 8 zeroes.
  base2digits = 0
  for i in xrange(0,1000):
      testNum = 2**i
      #print "testNum"
      #print testNum
      #print "numberToEncode"
      #print numberToEncode
      if (numberToEncode % testNum != 0):
          base2digits -= 1 
          break
      base2digits += 1
  return base2digits

def numTernaryDigitsEqual0(numberToEncode):
##start with 2 or 3 bits for base2, can represent up to 4 or 8 zeroes.
  base3digits = 0
  for i in xrange(0,1000):
      testNum = 3**i
      #print "testNum"
      #print testNum
      #print "numberToEncode"
      #print numberToEncode
      if (numberToEncode % testNum != 0):
          base3digits -= 1 
          break
      base3digits += 1
  return base3digits

def numFiveDigitsEqual0(numberToEncode):
  base5digits = 0
  for i in xrange(0,1000):
      testNum = 5**i
      #print "testNum"
      #print testNum
      #print "numberToEncode"
      #print numberToEncode
      if (numberToEncode % testNum != 0):
          base5digits -= 1 
          break
      base5digits += 1
  return base5digits
#integer log, finds number of decimal places in base 2 for given number
#gives a ceiling
def integerLog2Ceiling(inputNumber):
  numDigitsOfInput = 0
  while( inputNumber > 0):
    inputNumber = inputNumber >> 1
    numDigitsOfInput+=1
  print "numDigitsOfInput"
  print numDigitsOfInput
  return numDigitsOfInput

def integerLog2Floor(inputNumber):
  numDigitsOfInput = 0
  while( inputNumber > 0):
    inputNumber = inputNumber >> 1
    numDigitsOfInput+=1
  print "numDigitsOfInputFloor"
  print numDigitsOfInput
  return numDigitsOfInput

def integerLog10(inputNumber):
  numDigitsOfInput = 1
  while( inputNumber > 10):
    inputNumber = inputNumber / 10
    numDigitsOfInput+=1
  return numDigitsOfInput

#s = random.randint(0, 2**1000 - 1)
inputToEncode = random.randint(0, 2**1000 - 1)
#b = BitArray(uint=inputToEncode, length=1000)
#print b[0:5]
#print (b ^ b)[0:5]
#c = BitArray(uint=2, length=2)
#print b[0:2]^c
#primes = [3,5,7,11]

#permutation =  itertools.product([0,1,2,3],[0,1,2,3],[0,1,2,3],[0,1,2,3],[0,1,2,3],[0,1,2,3])
#permutation =  itertools.product([0,1],[0,1],[0,1],[0,1])
#permutation =  itertools.product([4,5,6,7],[4,5,6,7],[4,5,6,7],[4,5,6,7],[4,5,6,7],[4,5,6,7])
#permutation =  itertools.product([4,5,6,7],[4,5,6,7],[4,5,6,7])
#permutationList = []
#permutationList = list(permutation)
#print permutationList

totalSteps = 0
while (inputToEncode > 0):
  base2digits = numBinaryDigitsEqual0(inputToEncode)
  base3digits = numTernaryDigitsEqual0(inputToEncode)
  base5digits = numFiveDigitsEqual0(inputToEncode)
  twoPower = 2**base2digits
  threePower = 3**base3digits
  fivePower = 5**base5digits
  print "BEFORE"
  print inputToEncode 
  inputToEncode = inputToEncode - twoPower*threePower*fivePower
  print "AFTER"
  print inputToEncode
  print "base2digits"
  print base2digits
  print "base3digits"
  print base3digits
  print "base5digits"
  print base5digits
  totalSteps += 1
  print "totalSteps"
  print totalSteps
print totalSteps
  
  #for aTuple in permutationList:
  #   testNum = twoPower
  #   #testNum *= primes[0]**aTuple[0]
  #   #testNum *= primes[1]**aTuple[1]
  #   #testNum *= primes[2]**aTuple[2]
  #   #testNum *= primes[3]**aTuple[3]
  #   #testNum *= primes[4]**aTuple[4]
  #   #testNum *= primes[5]**aTuple[5]
  #   numDigitsOfInputToEncode = integerLog2Ceiling(testNum)
  #   numDigitsOfInputToShift = integerLog2Floor(testNum)
  #   print "testNum"
  #   print testNum 
  #   print "inputToEncode % (2**numDigitsOfInputToEncode)"
  #   print inputToEncode % (2**numDigitsOfInputToShift)
  #   print "power of 2 encoded"
  #   print (2**numDigitsOfInputToEncode)
  #   if (not (testNum - inputToEncode % (2**numDigitsOfInputToShift))):
  #      print "went through if"
  #      #print "bin(inputToEncode)"
  #      #print bin(inputToEncode)
  #      #print "numDigitsOfInputToEncode"
  #      #print numDigitsOfInputToEncode 
  #      #print "testNum"
  #      #print testNum 
  #      #print "inputToEncode % (2**numDigitsOfInputToEncode)"
  #      #print inputToEncode % (2**numDigitsOfInputToEncode)
  #      #print "shift"
  #      #print numDigitsOfInputToShift
  #      #print "bin(testNum)"
  #      #print bin(testNum)
  #      if (numDigitsOfInputToShift > biggestShift):
  #        biggestShift = numDigitsOfInputToEncode 
  #print "biggestShift"
  #print biggestShift 
  #numShift += 1
  #sumShift += biggestShift
  #inputToEncode = inputToEncode >> biggestShift
#print "sumShift"
#print sumShift
#print "numShift"
#print numShift




#for i in xrange(32):
#  for aTuple in permutationList:
#     testNum = twoPower
#     testNum *= pow(primes[0],aTuple[0])
#     testNum *= pow(primes[1],aTuple[1])
#     testNum *= pow(primes[2],aTuple[2])
#     numDigitsOfInputToEncode = integerLog10(testNum)
#     #print "testNum"
#     #print testNum 
#     #print "inputToEncode % (numDigitsOfInputToEncode*10)"
#     #print inputToEncode % (10**numDigitsOfInputToEncode)
#     if (not (testNum - inputToEncode % (10**numDigitsOfInputToEncode))):
#        print testNum
#        inputToEncode = inputToEncode >> numDigitsOfInputToEncode


#could try to invert the order shown here.
#print list(permutations)
#testNum = pow(2,encoded)
#for i in xrange(0,16):
#  testNum *= pow(primes[0],i)
#  for j in xrange(0,16):
#    testNum *= pow(primes[1],j)
#    for k in xrange(0,16):
#      testNum *= pow(primes[2],k)
#      base10digits = 1
#      while (testNum / (base10digits * 10) > 0):
#          base10digits+=1
#      if (not (testNum - (s % (base10digits*10)))):
#          print base10digits
#      print "random num"
#      print (s % (base10digits*10))
#      print "testNum"
#      print testNum
#      testNum = pow(2,encoded)
#print encoded

##start with 2 or 3 bits for base2, can represent up to 4 or 8 zeroes.
#permutation =  itertools.product([4,5,6,7],[4,5,6,7],[4,5,6,7])
#testNum = 1
#base2 = 0
#base2digits = 0
#for i in xrange(0,32):
#    testNum = pow(2,i)
#    if (inputToEncode % testNum != 0):
#        break
#    base2digits += 1
