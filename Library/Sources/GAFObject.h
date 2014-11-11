#pragma once

#include "GAFDelegates.h"
#include "GAFSprite.h"
#include "GAFCollections.h"

NS_GAF_BEGIN

class GAFAsset;
class GAFTimeline;

class GAFObject : public GAFSprite
{
private:
    cocos2d::AffineTransform GAF_CGAffineTransformCocosFormatFromFlashFormat(cocos2d::AffineTransform aTransform);

public:

    typedef std::vector<GAFObject*> DisplayList_t;
    typedef std::vector<GAFSprite*> SpriteList_t;
private:
    GAFSequenceDelegate_t                   m_sequenceDelegate;
    GAFAnimationFinishedPlayDelegate_t      m_animationFinishedPlayDelegate;
    GAFAnimationStartedNextLoopDelegate_t   m_animationStartedNextLoopDelegate;
    GAFFramePlayedDelegate_t                m_framePlayedDelegate;
    GAFObjectControlDelegate_t              m_controlDelegate;

    uint32_t                                m_totalFrameCount;
    uint32_t                                m_currentSequenceStart;
    uint32_t                                m_currentSequenceEnd;

    bool                                    m_isRunning;
    bool                                    m_isLooped;
    bool                                    m_isReversed;

    double                                  m_timeDelta;
    uint32_t                                m_fps;

    bool                                    m_animationsSelectorScheduled;

private:
    void constructObject();

protected:
    GAFAsset*                               m_asset;
    GAFTimeline*                            m_timeline;
    DisplayList_t                           m_displayList;
    DisplayList_t                           m_masksDList;
    SpriteList_t                            m_visibleObjects;
    GAFCharacterType                        m_charType;
    GAFObjectType                           m_objectType;
    uint32_t                                m_currentFrame;
    Filters_t                               m_parentFilters;
    cocos2d::Vec4                           m_parentColorTransforms[2];

    void    processAnimation();
    void    processAnimations(float dt);

    void    setAnimationRunning(bool value);
    void    instantiateObject(const AnimationObjects_t& objs, const AnimationMasks_t& masks);

    void    instantiateAnimatedObjects(const AnimationObjects_t &objs, int max);
    void    instantiateMasks(const AnimationMasks_t& masks);

    void    encloseNewTimeline(uint32_t reference, uint32_t objId);

    GAFObject();

public:

    /// @note do not forget to call setSequenceDelegate(nullptr) before deleting your subscriber
    void setSequenceDelegate(GAFSequenceDelegate_t delegate);

    /// @note do not forget to call setAnimationFinishedPlayDelegate(nullptr) before deleting your subscriber
    void setAnimationFinishedPlayDelegate(GAFAnimationFinishedPlayDelegate_t delegate);

    /// @note do not forget to call setAnimationStartedNextLoopDelegate(nullptr) before deleting your subscriber
    void setAnimationStartedNextLoopDelegate(GAFAnimationStartedNextLoopDelegate_t delegate);

    /// @note do not forget to call setFramePlayedDelegate(nullptr) before deleting your subscriber
    void setFramePlayedDelegate(GAFFramePlayedDelegate_t delegate);

    /// @note do not forget to call setControlDelegate(nullptr) before deleting your subscriber
    void setControlDelegate(GAFObjectControlDelegate_t delegate);

#if COCOS2D_VERSION < 0x00030200
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool flags)
#else
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
#endif
    {
        (void)flags;
        (void)renderer;
        (void)transform;
    }
public:
    // Playback accessing

    void        start();
    void        stop();
    void        step();
    void        pauseAnimation();
    void        resumeAnimation();

    bool        isDone() const;
    bool        getIsAnimationRunning() const;

    bool        isLooped() const;
    void        setLooped(bool looped);

    bool        isReversed() const;
    void        setReversed(bool reversed);

    uint32_t    getTotalFrameCount() const;
    uint32_t    getCurrentFrameIndex() const;

    bool        setFrame(uint32_t index);

    /// Plays specified frame and then stops	
    bool        gotoAndStop(const std::string& frameLabel);
    bool        gotoAndStop(uint32_t frameNumber);

    /// Plays animation from specified frame	
    bool        gotoAndPlay(const std::string& frameLabel);
    bool        gotoAndPlay(uint32_t frameNumber);

    uint32_t    getStartFrame(const std::string& frameLabel);
    uint32_t    getEndFrame(const std::string& frameLabel);

    /// Plays animation sequence with specified name
    /// @param name a sequence name
    /// @param looped if true - sequence should play in cycle
    /// @param resume if true - animation will be played immediately, if false - playback will be paused after the first frame is shown
    /// @param hint specific animation playback parameters

    bool playSequence(const std::string& name, bool looped = false, bool resume = true);
    void clearSequence();

public:

    virtual ~GAFObject();

    static GAFObject * create(GAFAsset * anAsset, GAFTimeline* timeline);

    bool init(GAFAsset * anAnimationData, GAFTimeline* timeline);

    bool hasSequences() const;

    cocos2d::Rect getBoundingBoxForCurrentFrame();

    virtual const cocos2d::Mat4& getNodeToParentTransform() const override;

    //////////////////////////////////////////////////////////////////////////
    // Accessors

    // Searches for an object by given string
    // @param object name e.g. "head" or object path e.g. "knight.body.arm"
    // @note it can slow down the real-time performance
    // @returns instance of GAFObject or null. Warning: the instance could be invalidated when the system catches EVENT_COME_TO_FOREGROUND event
    GAFObject* getObjectByName(const std::string& name);
    const GAFObject* getObjectByName(const std::string& name) const;
#if 0
    // Searches for an object by given id
    // @param object id
    // @note it can slow down the real-time performance
    // @returns instance of GAFSprite or null. Warning: the instance could be invalidated when the system catches EVENT_COME_TO_FOREGROUND event
    GAFSprite* getSprite(uint32_t id) const;

    // Searches for an object by given string
    // @param object name e.g. "head" or object path e.g. "knight.body.arm"
    // @note it can slow down the real-time performance
    // @returns GAFSpriteID
    //GAFSpriteID   getSpriteId(const std::string& name) const;

    // Searches for an animated object by given class name(linkage name)
    // @note the root object has a name "rootTimeline"
    // @param object name e.g. "Head"
    // @returns instance of GAFObject or null
    GAFObject* getSubObject(const std::string& className) const;

    // Searches for an animated object by given class name(linkage name)
    // @param object id
    // @returns instance of GAFObject or null
    GAFObject* getSubObject(uint32_t id) const;

    // Searches for an animated object by given class name(linkage name)
    // @note the root object has a name "rootTimeline"
    // @param object name e.g. "Head"
    // @returns object id
    uint32_t getSubObjectId(const std::string& className) const;
    //////////////////////////////////////////////////////////////////////////
#endif
    void realizeFrame(cocos2d::Node* out, size_t frameIndex);

    uint32_t getFps() const;

    void setFps(uint32_t value);
};

NS_GAF_END