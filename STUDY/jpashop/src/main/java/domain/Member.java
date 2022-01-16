package jpabook.jpashop;

import domain.Address;
import domain.Order;
import lombok.Getter;
import lombok.Setter;

import javax.persistence.*;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Entity
@Getter @Setter
public class Member {

    @Id @GeneratedValue
    @Column(name = "member_id")
    private Long id;
    private String username;

//    @Embedded
//    private Address address;
//
//    @OneToMany(mappedBy = "member") // 일대다, mapping 된 거울
//    private List<Order> orders = new ArrayList<>();

}
