package hello.core.annotaion;

import org.springframework.beans.factory.annotation.Qualifier;

import java.lang.annotation.*;

@Target({ElementType.FIELD, ElementType.METHOD, ElementType.PARAMETER, ElementType.TYPE, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
@Qualifier("mainDiscountPolicy")
/**어노테이션 생성*/
// 문자는 컴파일 타임에서 오류 탐색 불가능
// 사용하는 코드 추적 편리
public @interface MainDiscountPolicy {
}
