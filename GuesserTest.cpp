/**
 * Unit Tests for the Guesser class
 */

#include <gtest/gtest.h>
#include <string>
#include "Guesser.h"

using std::string;

class GuesserTest : public ::testing::Test
{
protected:
    GuesserTest(){} //constructor runs before each test
    virtual ~GuesserTest(){} //destructor cleans up after tests
    virtual void SetUp(){}  //sets up before each test (after constructor)
    virtual void TearDown(){} //clean up after each test, (before destructor)
};

// Example "smoke test" (can be deleted)
TEST(GuesserTest, DistanceShouldBeZeroWhenGuessMatchesSecretExactly)
{
    string secretWord = "Secret";
    string guessWord = "Secret";
    Guesser guesserObject(secretWord);
    unsigned int computedDistance = guesserObject.distance(guessWord);
    unsigned int expectedDistance = 0;
    ASSERT_EQ(computedDistance, expectedDistance);
}

TEST(GuesserTest, DistanceShouldCountSingleCharacterDifferenceInSameLengthStrings)
{
    string secretWord = "house";
    string guessWord = "horse";
    Guesser guesserObject(secretWord);
    unsigned int actualResult = guesserObject.distance(guessWord);
    unsigned int expectedResult = 1;
    ASSERT_EQ(actualResult, expectedResult);
}

TEST(GuesserTest, DistanceShouldCountMultipleCharacterDifferencesInSameLengthStrings)
{
    string secretWord = "abcd";
    string guessWord = "wxyz";
    Guesser guesserObject(secretWord);
    unsigned int actualDistance = guesserObject.distance(guessWord);
    unsigned int expectedDistance = 4;
    ASSERT_EQ(actualDistance, expectedDistance);
}

TEST(GuesserTest, DistanceShouldIncreaseWhenGuessIsShorterThanSecret)
{
    string secretWord = "abcd";
    string shortGuess = "abc";
    Guesser guesserObject(secretWord);
    unsigned int observedDistance = guesserObject.distance(shortGuess);
    unsigned int correctDistance = 1;
    ASSERT_EQ(observedDistance, correctDistance);
}

TEST(GuesserTest, DistanceShouldIncreaseByTwoWhenGuessIsTwoCharactersShorter)
{
    string secretWord = "abcd";
    string shortGuess = "ab";
    Guesser guesserObject(secretWord);
    unsigned int observedDistance = guesserObject.distance(shortGuess);
    unsigned int expectedValue = 2;
    ASSERT_EQ(observedDistance, expectedValue);
}

TEST(GuesserTest, DistanceShouldIncreaseWhenGuessIsLongerThanSecret)
{
    string secretWord = "abc";
    string longGuess = "abcd";
    Guesser guesserObject(secretWord);
    unsigned int actualValue = guesserObject.distance(longGuess);
    unsigned int expectedValue = 1;
    ASSERT_EQ(actualValue, expectedValue);
}

TEST(GuesserTest, DistanceShouldBeCappedAtSecretLengthWhenGuessIsMuchLonger)
{
    string secretWord = "abc";
    string longGuess = "abcdefghi";
    Guesser guesserObject(secretWord);
    unsigned int actualDistance = guesserObject.distance(longGuess);
    unsigned int expectedDistance = 3;
    ASSERT_EQ(actualDistance, expectedDistance);
}

TEST(GuesserTest, DistanceShouldCombineMismatchAndLengthDifferenceForShorterGuess)
{
    string secretWord = "abcd";
    string guessWord = "abX";
    Guesser guesserObject(secretWord);
    unsigned int actualDistance = guesserObject.distance(guessWord);
    unsigned int expectedDistance = 2; 
    ASSERT_EQ(actualDistance, expectedDistance);
}

TEST(GuesserTest, DistanceShouldCombineMismatchAndLengthDifferenceForLongerGuess)
{
    string secretWord = "abcd";
    string guessWord = "XbcdZ";
    Guesser guesserObject(secretWord);
    unsigned int actualDistance = guesserObject.distance(guessWord);
    unsigned int expectedDistance = 2;
    ASSERT_EQ(actualDistance, expectedDistance);
}

TEST(GuesserTest, ConstructorShouldStoreSecretNormallyWhenLengthIsUnderThirtyTwo)
{
    string secretWord = "short_secret";
    Guesser guesserObject(secretWord);
    unsigned int actualDistance = guesserObject.distance(secretWord);
    unsigned int expectedDistance = 0;
    ASSERT_EQ(actualDistance, expectedDistance);
}

TEST(GuesserTest, ConstructorShouldTruncateSecretWhenLengthExceedsThirtyTwoCharacters)
{
    string originalSecret = "12345678901234567890123456789012EXTRA_DATA";
    string truncatedSecret = "12345678901234567890123456789012";
    Guesser guesserObject(originalSecret);
    bool matchesTruncatedVersion = guesserObject.match(truncatedSecret);
    ASSERT_TRUE(matchesTruncatedVersion);
}

TEST(GuesserTest, ConstructorShouldAllowExactlyThirtyTwoCharacterSecret)
{
    string exactLengthSecret = "12345678901234567890123456789012";
    Guesser guesserObject(exactLengthSecret);
    bool matchResult = guesserObject.match(exactLengthSecret);
    ASSERT_TRUE(matchResult);
}

TEST(GuesserTest, MatchShouldReturnTrueForCorrectGuessOnFirstAttempt)
{
    string secretWord = "code";
    string guessWord = "code";
    Guesser guesserObject(secretWord);
    bool actualResult = guesserObject.match(guessWord);
    ASSERT_TRUE(actualResult);
}

TEST(GuesserTest, MatchShouldReturnFalseForIncorrectGuessWithDistanceLessThanOrEqualToTwo)
{
    string secretWord = "code";
    string guessWord = "cade";
    Guesser guesserObject(secretWord);
    bool result = guesserObject.match(guessWord);
    ASSERT_FALSE(result);
}

TEST(GuesserTest, MatchShouldStillAllowLaterCorrectGuessIfOnlyOneSmallMissOccurred)
{
    string secretWord = "code";
    string wrongGuess = "cade";
    string correctGuess = "code";
    Guesser guesserObject(secretWord);
    bool firstResult = guesserObject.match(wrongGuess);
    bool secondResult = guesserObject.match(correctGuess);
    ASSERT_FALSE(firstResult);
    ASSERT_TRUE(secondResult);
}

TEST(GuesserTest, MatchShouldLockAfterThreeConsecutiveIncorrectGuessesWithSmallDistances)
{
    string secretWord = "code";
    Guesser guesserObject(secretWord);
    bool firstAttempt = guesserObject.match("cade");
    bool secondAttempt = guesserObject.match("coda");
    bool thirdAttempt = guesserObject.match("cope");
    bool fourthAttempt = guesserObject.match("code");
    ASSERT_FALSE(firstAttempt);
    ASSERT_FALSE(secondAttempt);
    ASSERT_FALSE(thirdAttempt);
    ASSERT_FALSE(fourthAttempt);
}

TEST(GuesserTest, MatchShouldImmediatelyLockWhenDistanceIsGreaterThanTwo)
{
    string secretWord = "code";
    string farGuess = "xxxx";
    string correctGuess = "code";
    Guesser guesserObject(secretWord);
    bool firstResult = guesserObject.match(farGuess);
    bool secondResult = guesserObject.match(correctGuess);
    ASSERT_FALSE(firstResult);
    ASSERT_FALSE(secondResult);
}

TEST(GuesserTest, MatchShouldResetFailedGuessCountAfterSuccessfulMatch)
{
    string secretWord = "code";
    Guesser guesserObject(secretWord);
    bool firstAttempt = guesserObject.match("cade");   
    bool secondAttempt = guesserObject.match("coda");   
    bool thirdAttempt = guesserObject.match("code");    
    bool fourthAttempt = guesserObject.match("cade");   
    bool fifthAttempt = guesserObject.match("coda");    
    bool sixthAttempt = guesserObject.match("code");    
    ASSERT_FALSE(firstAttempt);
    ASSERT_FALSE(secondAttempt);
    ASSERT_TRUE(thirdAttempt);
    ASSERT_FALSE(fourthAttempt);
    ASSERT_FALSE(fifthAttempt);
    ASSERT_TRUE(sixthAttempt);
}

TEST(GuesserTest, MatchShouldNotLockIfSuccessfulGuessOccursBeforeThirdFailure)
{
    string secretWord = "hello";
    Guesser guesserObject(secretWord);
    bool result1 = guesserObject.match("jello"); 
    bool result2 = guesserObject.match("hello"); 
    bool result3 = guesserObject.match("cello"); 
    bool result4 = guesserObject.match("hello"); 
    ASSERT_FALSE(result1);
    ASSERT_TRUE(result2);
    ASSERT_FALSE(result3);
    ASSERT_TRUE(result4);
}

TEST(GuesserTest, MatchShouldReturnFalseEveryTimeAfterImmediateBruteForceLock)
{
    string secretWord = "apple";
    Guesser guesserObject(secretWord);
    bool firstAttempt = guesserObject.match("zzzzz");
    bool secondAttempt = guesserObject.match("apple");
    bool thirdAttempt = guesserObject.match("apple");
    ASSERT_FALSE(firstAttempt);
    ASSERT_FALSE(secondAttempt);
    ASSERT_FALSE(thirdAttempt);
}

TEST(GuesserTest, MatchShouldAllowUpToThreeConsecutiveMissesButNotAFourthAttempt)
{
    string secretWord = "test";
    Guesser guesserObject(secretWord);
    bool attempt1 = guesserObject.match("tent"); 
    bool attempt2 = guesserObject.match("text"); 
    bool attempt3 = guesserObject.match("best"); 
    bool attempt4 = guesserObject.match("test"); 
    ASSERT_FALSE(attempt1);
    ASSERT_FALSE(attempt2);
    ASSERT_FALSE(attempt3);
    ASSERT_FALSE(attempt4);
}

TEST(GuesserTest, DistanceShouldWorkWithEmptyGuessAgainstNonEmptySecret)
{
    string secretWord = "abc";
    string emptyGuess = "";
    Guesser guesserObject(secretWord);
    unsigned int actualDistance = guesserObject.distance(emptyGuess);
    unsigned int expectedDistance = 3;
    ASSERT_EQ(actualDistance, expectedDistance);
}

TEST(GuesserTest, DistanceShouldBeZeroForBothSecretAndGuessEmpty)
{
    string secretWord = "";
    string emptyGuess = "";
    Guesser guesserObject(secretWord);
    unsigned int actualDistance = guesserObject.distance(emptyGuess);
    unsigned int expectedDistance = 0;
    ASSERT_EQ(actualDistance, expectedDistance);
}

TEST(GuesserTest, MatchShouldReturnTrueWhenBothSecretAndGuessAreEmptyStrings)
{
    string secretWord = "";
    string emptyGuess = "";
    Guesser guesserObject(secretWord);
    bool actualResult = guesserObject.match(emptyGuess);
    ASSERT_TRUE(actualResult);
}