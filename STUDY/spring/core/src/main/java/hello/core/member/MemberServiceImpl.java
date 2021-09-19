package hello.core.member;

public class MemberServiceImpl implements MemberService{

    private final MemberReposiory memberReposiory;

    public MemberServiceImpl(MemberReposiory memberReposiory) {
        this.memberReposiory = memberReposiory;
    }

    @Override
    public void join(Member member) {
        memberReposiory.save(member);
    }

    @Override
    public Member findMember(Long memberId) {
        return memberReposiory.findById(memberId);
    }
}
