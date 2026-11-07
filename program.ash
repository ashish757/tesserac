int limit = 15;
int i = 1;

print "Starting FizzBuzz:";

while (i < limit + 1) {
    if (i % 15 == 0) {
        print "FizzBuzz";
    } else if (i % 3 == 0) {
        print "Fizz";
    } else if (i % 5 == 0) {
        print "Buzz";
    } else {
        print i;
    }
    i++;
}