class lOperations {
  public static long main(String[] s) {
    long a = 52, b = 43, c, d = 0, e = 1;
    c = a - b;
    System.out.println(c);
    c = a * b;
    System.out.println(c);
    c = a / b;
    System.out.println(c);
    c = a % b;
    System.out.println(c);
    if(a > b){
      c = -a;
      System.out.println(c);
    }
    c = a + b;
    System.out.println(c);
    return c;
  }
}