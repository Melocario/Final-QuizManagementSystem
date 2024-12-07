class AdminInterface {
public:
    virtual void addUser() = 0;
    virtual void viewUsers() const = 0;
    virtual void editUser() = 0;
    virtual void deleteUser() = 0;
    virtual void searchUser() const = 0;
    virtual void sortUsers() = 0;

    virtual ~AdminInterface() {}
};
