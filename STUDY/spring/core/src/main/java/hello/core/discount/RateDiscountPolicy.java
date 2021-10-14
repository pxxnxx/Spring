package hello.core.discount;

import hello.core.annotaion.MainDiscountPolicy;
import hello.core.member.Grade;
import hello.core.member.Member;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Component;

@Component
//@Primary // 같은 타입의 빈 있을 경우 우선순위 가짐 . 메인을 @Primary 서브를 @Qualifier 로 사용할 수 있음
//@Qualifier("mainDiscountPolicy") // 문자이기 때문에 컴파일 타임에 오류 찾을 수 없음
@MainDiscountPolicy // 그래서 직접 어노테이션 생성
public class RateDiscountPolicy implements DiscountPolicy{

    private int discountPercent = 10;

    @Override
    public int discount(Member member, int price) { // cmd shift t
        if (member.getGrade() == Grade.VIP) {
            return price * discountPercent / 100;
        }
        else {
            return 0;
        }
    }
}
