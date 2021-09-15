package hello.core.member;

public interface MemberReposiory {

    void save(Member member);

    Member findById(Long memberId);
}
