package hellojpa;

import javax.persistence.*;
import java.util.List;

public class JpaMain {
    public static void main(String[] args) {
        EntityManagerFactory emf = Persistence.createEntityManagerFactory("hello");

        EntityManager em = emf.createEntityManager();

        EntityTransaction tx = em.getTransaction();
        tx.begin();

//        try {
//            Member member = em.find(Member.class, 160L);
//            member.setName("AAAA");
//
//            em.detach(member);
//
//            tx.commit();
//        } catch (Exception e) {
//            e.printStackTrace();
//            tx.rollback();
//        } finally {
//            em.close();
//        }

        emf.close();

    }

}

