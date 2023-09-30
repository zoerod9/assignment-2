import java.util.Random;

public class Main {

    static class SortingAvgInput {
        double average;
        double[] array;

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
            System.out.println("my input avg: " + input.getAverage());
            input.setAverage(8);

        }
    }

    static class MergingAvg implements Runnable {
        @Override
        public void run() {
            System.out.println("MergingAvg!");
        }
    }

    void sort(int input[]) {
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
            int temp = input[minIndex];
            input[minIndex] = input[i];
            input[i] = temp;
        }
    }

    double getAverage(double[] input) {
        double result = 0;
        for (int i = 0; i < input.length; i++) {
            result = result + input[i];
        }

        return result / input.length;
    }

    double randomNumber() {
        Random random = new Random();
        return random.nextDouble();
    }

    public static void main(String[] args) throws InterruptedException {

        SortingAvgInput input1 = new SortingAvgInput();
        input1.setAverage(5);

        Runnable runnable = new SortingAvg(input1);
        Thread t = new Thread(runnable);

        t.start();
        t.join();

        System.out.println("my output avg" + input1.getAverage());

    }
}