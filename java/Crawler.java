public class Crawler{

	public static void main(String[] args) {
       System.out.println("我说出一句话了。");
       test();
    }

    public static void test(){
    	Document document = 
    	Jsoup.connect("http://politics.people.com.cn/GB/1024/")
    	.get();
    	System.out.println(document.html());
    }


}