// Zoe Rodriguez
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

        public MergngAvgInput(double[] array1, double[] array2, double[] resultArray, double average1,
                double average2) {
            this.array1 = array1;
            this.array2 = array2;
            this.resultArray = resultArray;
            this.average1 = average1;
            this.average2 = average2;
        }

        public double[] getArray1() {
            return array1;
        }

        public void setArray1(double[] array1) {
            this.array1 = array1;
        }

        public double[] getArray2() {
            return array2;
        }

        public void setArray2(double[] array2) {
            this.array2 = array2;
        }

        public double[] getResultArray() {
            return resultArray;
        }

        public void setResultArray(double[] resultArray) {
            this.resultArray = resultArray;
        }

        public double getAverage1() {
            return average1;
        }

        public void setAverage1(double average1) {
            this.average1 = average1;
        }

        public double getAverage2() {
            return average2;
        }

        public void setAverage2(double average2) {
            this.average2 = average2;
        }

        public double getCombinedAvg() {
            return combinedAvg;
        }

        public void setCombinedAvg(double combinedAvg) {
            this.combinedAvg = combinedAvg;
        }
    }

    static class SortingAvg implements Runnable {

        SortingAvgInput input;

        public SortingAvg(Main.SortingAvgInput input) {
            this.input = input;
        }

        @Override
        public void run() {
            sort(input.getArray());
            input.setAverage(getAverage(input.getArray()));
        }
    }

    static class MergingAvg implements Runnable {
        private Main.MergngAvgInput input;

        public MergingAvg(Main.MergngAvgInput input) {
            this.input = input;
        }

        @Override
        public void run() {

            int n1 = input.getArray1().length;

            int i = 0, j = 0, k = 0;

            while (i < n1 && j < n1) {
                if (input.getArray1()[i] < input.getArray2()[j])
                    input.getResultArray()[k++] = input.getArray1()[i++];
                else
                    input.getResultArray()[k++] = input.getArray2()[j++];
            }

            while (i < n1)
                input.getResultArray()[k++] = input.getArray1()[i++];

            while (j < n1)
                input.getResultArray()[k++] = input.getArray2()[j++];

        }
    }

    static void sort(double input[]) {
        int length = input.length;
        for (int i = 0; i < length - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < length; j++)
                if (input[j] < input[minIndex])
                    minIndex = j;
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

        // One thread case
        long startOneThread = System.currentTimeMillis();
        SortingAvgInput input = new SortingAvgInput(b);

        Runnable runnable = new SortingAvg(input);
        Thread t = new Thread(runnable, "EntireArrayThread");

        t.start();
        t.join();

        long endOneThread = System.currentTimeMillis();
        System.out.println("Sorting in one thread is done in: " + (endOneThread - startOneThread) + "ms");
        for (int i = 0; i < input.getArray().length; i++) {
            if (i == 10) {
                break;
            }
            System.out.println(input.getArray()[i]);
        }

        // Two threads case

        double[] aFirstHalf = new double[desiredLength / 2];
        double[] aSecondHalf = new double[desiredLength / 2];
        double[] mergedarray = new double[desiredLength];

        for (int i = 0; i < (desiredLength / 2); i++) {
            aFirstHalf[i] = a[i];
        }
        for (int i = 0; i < (desiredLength / 2); i++) {
            aSecondHalf[i] = a[i + desiredLength / 2];
        }

        long startTwoThreads = System.currentTimeMillis();
        SortingAvgInput inputFirstHalf = new SortingAvgInput(aFirstHalf);

        Runnable runnableFirstHalf = new SortingAvg(inputFirstHalf);
        Thread t1 = new Thread(runnableFirstHalf, "firstHalfThread");

        SortingAvgInput inputSecondHalf = new SortingAvgInput(aSecondHalf);

        Runnable runnableSecondHalf = new SortingAvg(inputSecondHalf);
        Thread t2 = new Thread(runnableSecondHalf, "secondHalfThread");

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        MergngAvgInput mergingAvgInput = new MergngAvgInput(inputFirstHalf.getArray(), inputSecondHalf.getArray(),
                mergedarray, inputFirstHalf.getAverage(), inputSecondHalf.getAverage());

        Runnable runnableMerge = new MergingAvg(mergingAvgInput);
        Thread tMerge = new Thread(runnableMerge, "mergeThread");

        tMerge.start();
        tMerge.join();
        long endTwoThreads = System.currentTimeMillis();

        System.out.println("Sorting in two threads is done in: " + (endTwoThreads - startTwoThreads) + "ms");
        for (int i = 0; i < mergingAvgInput.getResultArray().length; i++) {
            if (i == 10) {
                break;
            }
            System.out.println(mergingAvgInput.getResultArray()[i]);
        }

    }
}