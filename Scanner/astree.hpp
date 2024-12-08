#ifndef ASTREE_H
#define ASTREE_H

#include <algorithm> //std::next
#include <sstream> //std::stringstream
#include <memory>
#include <list>
#include <type_traits>

#define DEBUGAST(msg)
//#include <iostream>
//#define DEBUGAST(msg) std::cout <<__LINE__ <<"\t[" <<__func__ <<"]\t" <<msg <<std::endl;


namespace ASTree {


static int GlobalIdForASTNode = 0;

template <typename NodeType, typename = typename std::enable_if< std::is_enum<NodeType>::value >::type>
class ASTNode: public std::enable_shared_from_this<ASTNode<NodeType>> {
public:
    using WeakPtr = std::weak_ptr<ASTNode<NodeType>>;
    using SharedPtr = std::shared_ptr<ASTNode<NodeType>>;

private:
    explicit ASTNode() noexcept
        : ASTNode(static_cast<NodeType>(0), std::string())
    {}

    explicit ASTNode(NodeType type, std::string text) noexcept
        : type_(std::move(type))
        , text_(std::move(text))
        , parent_(SharedPtr())
    {
        DEBUGAST(std::string("ASTNode(): '") <<toString())
        GlobalIdForASTNode++;
    }


    template <typename NodeType_>
    struct MakeSharedEnabler: public ASTNode<NodeType_> {
        explicit MakeSharedEnabler(NodeType_ type, std::string text)
                : ASTNode(std::move(type), std::move(text)){}
    };

public:

    ASTNode(const ASTNode &) = delete;

    ASTNode(ASTNode &&rVal) noexcept = default;

    ~ASTNode(){
        DEBUGAST(std::string("~ASTNode(): '") <<toString())
    }

    static SharedPtr GetNewInstance(NodeType type, std::string text, const SharedPtr &child1, const SharedPtr &child2){
        const SharedPtr instance(std::make_shared<MakeSharedEnabler<NodeType>>(std::move(type), std::move(text)));
        if(child1){
            instance->addChild(child1);
        }
        if(child2){
            instance->addChild(child2);
        }
        return instance;
    }

    static SharedPtr GetNewInstance(NodeType type, const SharedPtr &child1, const SharedPtr &child2){
        return GetNewInstance(std::move(type), std::to_string(type), child1, child2);
    }

    static SharedPtr GetNewInstance(NodeType type, std::string text, const SharedPtr &child1){
        return GetNewInstance(std::move(type), std::move(text), child1, SharedPtr());
    }

    static SharedPtr GetNewInstance(NodeType type, const SharedPtr &child1){
        return GetNewInstance(std::move(type), std::to_string(type), child1);
    }

    static SharedPtr GetNewInstance(NodeType type, std::string text){
        return GetNewInstance(std::move(type), std::move(text), SharedPtr(), SharedPtr());
    }

    static SharedPtr GetNewInstance(NodeType type){
        return GetNewInstance(std::move(type), std::to_string(type));
    }

    // Добавить дочерний узел к текущему узлу
    void addChild(const SharedPtr child){
        DEBUGAST("adding child: " <<child->toString() <<" to root: " <<toString())

        if (child->isParentValid()) {
            DEBUGAST("Parent valid in child: "<<child->toString())
            child->getParent().lock()->removeChild(child);
        }

        DEBUGAST("Count of childs in " <<toString() <<" " <<childs_.size())
        auto self = this->shared_from_this(); //shared_from_this() we cannot use here,
                                    //because this object has not been initialized
                                    //yet and there isn't any shared_ptr that point to it
        child->setParent(self);

        childs_.remove(child); 
        childs_.emplace_back(std::move(child));
    }


    void removeChild(const SharedPtr &child){
        childs_.remove(child);

        
        if(child->getParent().lock() == this->shared_from_this()){
            child->setParent(SharedPtr());
        }
    }

    
    size_t getChildsCount() const { return childs_.size(); }

   
    bool isParentValid () const {
        //return (! parent_.owner_before(WeakPtr{})) && (! WeakPtr{}.owner_before(parent_));
        return parent_.lock() != nullptr;
    }

    
    WeakPtr getParent() { return parent_; }

   
    void setParent( const WeakPtr &parent) { parent_ = parent; }

    
    void setAsParent(const SharedPtr &val) {
        auto self = this->shared_from_this();
        val->addChild(self);
    }

    
    SharedPtr getChild(long index) const {
        return *(std::next(childs_.begin(), index));
    }

    
    // Возвращается пара, где левое значение -- нашелся ли такой узел среди дочерних, правое -- индекс, если нашелся
    std::pair<bool, long> getChildIndex(const SharedPtr &child) const {
        // Find given element in list
        const auto &it = std::find(childs_.begin(), childs_.end(), child);

        if (it == childs_.end()) {
            return std::make_pair(false, -1L);
        }

        return std::make_pair(true, std::distance(childs_.begin(), it));
    }

    
    std::pair<bool, long> getChildIndexInParent() {
        if(! isParentValid()){
            return std::make_pair(false, -1L);
        }

        const auto self = this->shared_from_this();
        return parent_.lock()->getChildIndex(self);
    }

    
    std::list<SharedPtr> getChilds() const { return childs_; }

   
    std::string getUniqueName() const { return uniqName_; }

    
    NodeType getType() const { return type_; }

    
    std::string getText() const { return text_; }

    
    std::string toString() const { return text_ + " (type: " + std::to_string(type_) + ")"; }

private:

    
    std::string uniqName_ = "node_" + std::to_string(GlobalIdForASTNode);
    
    
     NodeType type_;

     
     std::string text_;

     // родительский узел для данного узла дерева
     WeakPtr parent_;

     // потомки (ветви) данного узла дерева
     std::list<SharedPtr> childs_;
};

template <typename NodeType, typename = typename std::enable_if< std::is_enum<NodeType>::value >::type>
class ASTNodeWalker{
public:
    explicit ASTNodeWalker(std::shared_ptr<ASTNode<NodeType>> head) noexcept
        : head_(std::move(head))
    {}

    // сгенерирует файл для утилиты Graphviz (https://www.graphviz.org/)
    // которая сможет отобразить дерево
    void buildDotFormat(){
        std::stringstream stream;
        stream << "graph {" << std::endl;
        stream << "\tnode[fontsize=10,margin=0,width=\".4\", height=\".3\"];" << std::endl;
        stream << "\tsubgraph cluster17{" << std::endl;

        ASTNodeWalker::GetStringSubTree(head_, stream);

        stream << "\t}\n}" << std::endl;
        dotFormat_ = stream.str();
    }

    std::string getDotFormat() const {
        return dotFormat_;
    }

private:
    static void GetStringSubTree(const std::shared_ptr<ASTNode<NodeType>> &node, std::stringstream &stream){

        if(! node->getParent().lock()){
            stream << "\t\tn" << node->getUniqueName() << "[label=\"" << node->getText() <<"\"];" << std::endl;
        }else {
            stream << "\t\tn" << node->getUniqueName() << "[label=\"" << node->getText() <<"\"];" << std::endl;
            stream << "\t\tn" <<node->getParent().lock()->getUniqueName() << "--n" <<node->getUniqueName() << ";" << std::endl;
        }

        for (int i = 0; i < node->getChildsCount(); i++) {
            GetStringSubTree(node->getChild(i), stream);
        }
    }

private:
    const std::shared_ptr<ASTNode<NodeType>> head_;
    std::string dotFormat_;
};


}

#endif // ASTREE_H
