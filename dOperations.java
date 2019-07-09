class dOperations {
  public static double main(String[] s) {
    double a = 5.2, b = 4.3, c, d = 0.0, e = 1.0;
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