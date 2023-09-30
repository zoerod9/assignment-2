import java.util.Random;

public class Main {

    static class SortingAvgInput {
        double average;
        double[] array;

        public SortingAvgInput(double[] array) {
            this.average = 0;
            this.array = array;
        }

        public double getAverage() {
            return average;
        }

        public void setAverage(double average) {
            this.average = average;
        }

        public double[] getArray() {
            return array;
        }

        public void setArray(double[] array) {
            this.array = array;
        }
    }

    static class MergngAvgInput {
        double[] array1;
        double[] array2;
        double[] resultArray;
        double average1;
        double average2;
        double combinedAvg;
    }

    static class SortingAvg implements Runnable {

        SortingAvgInput input;

        public SortingAvg(Main.SortingAvgInput input) {
            this.input = input;
        }

        @Override
        public void run() {
            System.out.println("SortingAvg!");

            System.out.println("IN thread");
            for (int i = 0; i < input.getArray().length; i++) {
                System.out.println(input.getArray()[i]);
            }

            sort(input.getArray());
            input.setAverage(getAverage(input.getArray()));
        }
    }

    static class MergingAvg implements Runnable {
        @Override
        public void run() {
            System.out.println("MergingAvg!");
        }
    }

    static void sort(double input[]) {
        int n = input.length;

        // One by one move boundary of unsorted subarray
        for (int i = 0; i < n - 1; i++) {
            // Find the minimum element in unsorted array
            int minIndex = i;
            for (int j = i + 1; j < n; j++)
                if (input[j] < input[minIndex])
                    minIndex = j;

            // Swap the found minimum element with the first
            // element
            double temp = input[minIndex];
            input[minIndex] = input[i];
            input[i] = temp;
        }
    }

    static double getAverage(double[] input) {
        double result = 0;
        for (int i = 0; i < input.length; i++) {
            result = result + input[i];
        }

        return result / input.length;
    }

    static double randomNumber() {
        Random random = new Random();
        return (random.nextDouble() * 999) + 1;
    }

    public static void main(String[] args) throws InterruptedException {

        int desiredLength = Integer.parseInt(args[0]);

        double[] a = new double[desiredLength];
        double[] b = new double[desiredLength];

        // for each item in array,
        for (int i = 0; i < desiredLength; i++) {
            // insert random number between 1 and 1000 into arrayToSort's index
            a[i] = randomNumber();
        }

        // copy a into b
        for (int j = 0; j < desiredLength; j++) {
            b[j] = a[j];
        }

        SortingAvgInput input = new SortingAvgInput(b);

        Runnable runnable = new SortingAvg(input);
        Thread t = new Thread(runnable);

        System.out.println("Before thread");
        for (int i = 0; i < desiredLength; i++) {
            System.out.println(input.getArray()[i]);
        }
        // System.out.println(input.getAverage());

        t.start();
        t.join();

        System.out.println("After thread");
        for (int i = 0; i < desiredLength; i++) {
            System.out.println(input.getArray()[i]);
        }
        // System.out.println(input.getAverage());

    }
}