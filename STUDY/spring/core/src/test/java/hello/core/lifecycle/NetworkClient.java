package hello.core.lifecycle;

import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;

public class NetworkClient/* implements InitializingBean, DisposableBean */{

    private String url;

    public NetworkClient() {
        System.out.println("생성자 호출, url = " + url);
        connect();
        call("초기화 연결 메시지");
    }

    public void setUrl(String url) {
        this.url = url;
    }

    //서비스 시작시 호출
    public void connect() {
        System.out.println("url = " + url);
    }

    public void call(String message) {
        System.out.println("call = " + url + " message = " + message);
    }

    //서비스 종료시 호출
    public void disconnect() {
        System.out.println("close " + url);
    }

    /** Annotaion 이용 초기화, 소멸**/
    // 권장 하는 방법
    // 자바 코드면 어디든지 사용 가능
    // 컴포넌트 스캔과 잘 어울림
    // 유일한 단점 : 외부 라이브러리에는 적용하지 못함
    @PostConstruct
    public void init() {
        System.out.println("NetworkClient.init");
        connect();
        call("초기화 연결 메시지");
    }
    @PreDestroy
    public void close() {
        System.out.println("NetworkClient.close");
        disconnect();
    }


    /** 빈 등록 초기화, 소멸 */
//    public void init() {
//        System.out.println("NetworkClient.init");
//        connect();
//        call("초기화 연결 메시지");
//    }
//    // 메소드 이름 자유로움
//    // 스프링 코드 의존 X
//    // 외부 라이브러리도 가능
//    // @Bean의 destroyMethod는 기본값이 (inferred)이다. close, shutdown 이라는 메서드를 추론해서 자동 호출해줌
//    public void close() {
//        System.out.println("NetworkClient.close");
//        disconnect();
//    }


    /** 빈 인터페이스 등록 */
//    @Override
//    public void afterPropertiesSet() throws Exception {
//        connect();
//        call("초기화 연결 메시지");
//    }
//
//    // 초기화 소멸 메서드 이름 변경할 수 없음
//    // 외부 라이브러리는 코드를 변경할 수 없어서 고칠 수 없음
//    // 권장 X
//    @Override
//    public void destroy() throws Exception {
//        disconnect();
//    }
}
