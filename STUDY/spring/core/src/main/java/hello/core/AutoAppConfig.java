package hello.core;

import hello.core.member.MemberRepository;
import hello.core.member.MemoryMemberRepository;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;

@Configuration
//Component 붙은 Class 모두 스캔
@ComponentScan(
//        basePackages = "hello.core.member", // 탐색할 패키지의 시작 위치를 지정
        // 패키지 위치 지정하지 않고 설정 정보 클래스를 프로젝트 최상단에 두는것을 권장
        excludeFilters = @ComponentScan.Filter(type = FilterType.ANNOTATION, classes = Configuration.class)
        // 수동으로 만든 Configuration 제외하기 위함. (기존 코드 유지하기 위함)
        // Test Configuration 등록 막아줌
        )

public class AutoAppConfig {

//        @Bean(name = "memoryMemberRepository")
//        MemberRepository memberRepository() {
//                return new MemoryMemberRepository();
//        }
        // Overriding bean definition for bean 'memoryMemberRepository' 원래는 수동 빈이 우선순위 가지나 스프링부트에서는 충돌 시 오류 나도록 변경.
}
