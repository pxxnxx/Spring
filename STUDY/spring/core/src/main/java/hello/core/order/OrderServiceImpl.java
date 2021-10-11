package hello.core.order;

import hello.core.discount.DiscountPolicy;
import hello.core.member.Member;
import hello.core.member.MemberRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class OrderServiceImpl implements OrderService {

    private final MemberRepository memberRepository;
    private final DiscountPolicy discountPolicy;

    /** 필드 주입*/
    // 외부에서 변경 불가능하여 테스트 하기 힘듦
    // DI 프레임워크 없으면 아무것도 할 수 없음
    // 스프링 설정 및 테스트 용도 이외로 사용하지 말 것
//    @Autowired private MemberRepository memberRepository;
//    @Autowired private DiscountPolicy discountPolicy;

    /** 수정자 */
    // final 제거해야 함
    // 생성자는 생성될 때 바로 의존관계 주입되지만 수정자는 의존관계 주입 단계에 실행
    // 수정자 있으면 생성자 굳이 만들지 않아도 됨.
    // 선택, 변경 가능성 있는 의존관계에 사용
    // 주입할 대상이 없으면 오류 발생. 주입할 대상 없어도 동작하게 하려면 required = false
    // 주로 사용 X
//    @Autowired(required = false)
//    public void setDiscountPolicy(DiscountPolicy discountPolicy) {
//        System.out.println("discountPolicy = " + discountPolicy);
//        this.discountPolicy = discountPolicy;
//    }
//    @Autowired
//    public void setMemberRepository(MemberRepository memberRepository) {
//        System.out.println("memberRepository = " + memberRepository);
//        this.memberRepository = memberRepository;
//    }

    /** 생성자 주입*/
    // OrderServiceImpl 등록 -> 생성자 실행 -> Autowired 단계로 의존관계 주입
    // 생성자 호출 시점에서 한번만 호출되며 불변,필수 의존관계에 사용
    // 생성자가 하나라면 Autowired 생략 가능
    // final 사용 가능. final은 초기값 실수도 잡아줄 수 있음
    @Autowired
    public OrderServiceImpl(MemberRepository memberRepository, DiscountPolicy discountPolicy) {
        this.memberRepository = memberRepository;
        this.discountPolicy = discountPolicy;
    }

    /**일반 메서드 주입*/
    // 생성자와 비슷함
    // 일반적으로 사용 X
//    @Autowired
//    public void init(MemberRepository memberRepository, DiscountPolicy discountPolicy) {
//        this.memberRepository = memberRepository;
//        this.discountPolicy = discountPolicy;
//    }

    @Override
    public Order createOrder(Long memberId, String itemName, int itemPrice) {
        Member member = memberRepository.findById(memberId);
        int discountPrice = discountPolicy.discount(member, itemPrice);

        return new Order(memberId, itemName, itemPrice, discountPrice);
    }

    //테스트
    public MemberRepository getMemberRepository() {
        return memberRepository;
    }
}
