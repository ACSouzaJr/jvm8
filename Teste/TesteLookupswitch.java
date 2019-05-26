public class TesteLookupswitch {
	public static void main(String[] args) {
		simpleswitch(28);
	}

    public static int simpleswitch(int i) {
       switch (i) {
            case 1:
                return 10;
            case 10:
                return 100;
            default:
                return -42;
       }
    }
}