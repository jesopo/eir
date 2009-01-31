#ifndef client_h
#define client_h

#include "privilege.h"

#include <string>
#include <tr1/memory>

#include <paludis/util/private_implementation_pattern.hh>
#include <paludis/util/wrapped_forward_iterator.hh>

namespace eir
{
    struct Channel;
    typedef std::tr1::shared_ptr<Channel> ChannelPtr;
    struct Client;
    typedef std::tr1::shared_ptr<Client> ClientPtr;
    struct Membership;
    typedef std::tr1::shared_ptr<Membership> MembershipPtr;

    struct Client : private paludis::PrivateImplementationPattern<Client>,
                    private paludis::InstantiationPolicy<Client, paludis::instantiation_method::NonCopyableTag>,
                    public std::tr1::enable_shared_from_this<Client>
    {
        const std::string& nick() const;
        const std::string& user() const;
        const std::string& host() const;

        struct AttributeIteratorTag;
        typedef paludis::WrappedForwardIterator<AttributeIteratorTag,
                        std::pair<const std::string, std::string> > AttributeIterator;

        AttributeIterator attr_begin();
        AttributeIterator attr_end();

        std::string attr(const std::string &);
        void set_attr(const std::string &, const std::string &);

        Client(std::string, std::string, std::string);
        ~Client();

        void join_chan(ChannelPtr);
        void leave_chan(ChannelPtr);
        void leave_chan(MembershipPtr);

        struct ChannelIteratorTag;
        typedef paludis::WrappedForwardIterator<ChannelIteratorTag, const MembershipPtr> ChannelIterator;
        ChannelIterator begin_channels();
        ChannelIterator end_channels();

        PrivilegeSet& privs();

        typedef std::tr1::shared_ptr<Client> ptr;
    };

    struct Channel : private paludis::PrivateImplementationPattern<Channel>,
                     private paludis::InstantiationPolicy<Channel, paludis::instantiation_method::NonCopyableTag>,
                     public std::tr1::enable_shared_from_this<Channel>
    {
        const std::string& name();

        struct MemberIteratorTag;
        typedef paludis::WrappedForwardIterator<MemberIteratorTag, const MembershipPtr> MemberIterator;
        MemberIterator begin_members();
        MemberIterator end_members();

        bool add_member(MembershipPtr);
        bool remove_member(MembershipPtr);

        Channel(std::string);
        ~Channel();

        typedef std::tr1::shared_ptr<Channel> ptr;
    };

    struct Membership : private paludis::InstantiationPolicy<Membership, paludis::instantiation_method::NonCopyableTag>
    {
        Client::ptr client;
        Channel::ptr channel;
        int flags;

        enum {
            op = 1,
            voice = 2
        };

        bool is_op() { return (flags & op) != 0; }
        bool is_voice() { return (flags & voice) != 0; }

        typedef std::tr1::shared_ptr<Membership> ptr;

        Membership(Client::ptr cl, Channel::ptr ch)
            : client(cl), channel(ch)
        { }
    };
}

#endif
