package hello.core.member;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class MemberServiceImpl implements MemberService{

    private final MemberRepository memberRepository;

    @Autowired // ac.getBean(MemberRepository.class) 코드가 자동으로 들어간다고 생각
    // 의존관계 자동 주입 하기 위함. 생성자에 붙여주면 MemberRepository Type에 맞는 것들을 자동 주입해줌
    // 복습 할 것
    public MemberServiceImpl(MemberRepository memberRepository) {
        this.memberRepository = memberRepository;
    }

    @Override
    public void join(Member member) {
        memberRepository.save(member);
    }

    @Override
    public Member findMember(Long memberId) {
        return memberRepository.findById(memberId);
    }

    //Test
    public MemberRepository getMemberRepository() {
        return  memberRepository;
    }
}
