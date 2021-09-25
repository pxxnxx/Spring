package hello.core.singleton;

public class SingletonService {

    private static final SingletonService instance = new SingletonService();

    public static SingletonService getInstance() {
        return instance;
    }

    private SingletonService() { //외부에서 생성 불가능하게 private 생성자
    }

    public void logic() {
        System.out.println("싱긑톤 객체 로직 호출");
    }
}
