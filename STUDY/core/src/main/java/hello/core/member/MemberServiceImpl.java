package hello.core.member;

public class MemberServiceImpl implements MemberService{

    private final MemberReposiory memberReposiory = new MemoryMemberRepository();
    @Override
    public void join(Member member) {
        memberReposiory.save(member);
    }

    @Override
    public Member findMember(Long memberId) {
        return memberReposiory.findById(memberId);
    }
}
