package hello.core;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.FilterType;

@Configuration
//Component 붙은 Class 모두 스캔
@ComponentScan(
        excludeFilters = @ComponentScan.Filter(type = FilterType.ANNOTATION, classes = Configuration.class)
        // 수동으로 만든 Configuration 제외하기 위함. (기존 코드 유지하기 위함)
        // Test Configuration 등록 막아줌
        )

public class AutoAppConfig {
}
