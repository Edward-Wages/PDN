from mrjob.job import MRJob
import re
from pyparsing import line

#Your assignment is to write a MapReduce program using MRJob to find the longest word
#starting with each letter from a to z. If multiple words have the same length, they should be all
#listed. A part of expected result is shown below:

#"r" ["recommendations", "representations"]
#"s" ["superciliousness"]
#"t" ["thoughtlessness"]
#"g" ["gentlemanlike", "gratification"]
#"h" ["hertfordshire", "healthfulness"]

# Program is ran like so: python find_longest_words_MRJob.py 1342-0.txt

WORD_RE = re.compile(r"[a-zA-Z]+")


class MRLongestWord(MRJob):

    def mapper(self, _, line):
        words = WORD_RE.findall(line)
        for word in words:
            # process each word. Yields the first letter and the word itself
            yield word[0].lower(), word.lower()

    
    def reducer(self, letter, words):
        max_length = 0
        longest_words = set()  # Use a set to avoid duplicates

        for w in words:
            length = len(w)
            if length > max_length:
                max_length = length
                longest_words = {w}  # Start a new set with the current longest word
            elif length == max_length:
                longest_words.add(w)  # Add to the set of longest words

        # Yield the letters and the longest words for each letter found
        yield letter, list(longest_words)

if __name__ == '__main__':
    MRLongestWord.run()