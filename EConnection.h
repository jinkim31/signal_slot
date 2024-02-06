#ifndef ETHREAD2_ECONNECTION_H
#define ETHREAD2_ECONNECTION_H

#include <iostream>
#include <memory>
#include <typeindex>

class EObject;

class EConnection
{
public:
    enum ConnectionType{
        AUTO,
        QUEUED,
        DIRECT,
    };
    struct GeneralizedConnection
    {
        GeneralizedConnection(
                EObject *signalObject, const std::string& signalId,
                EObject *slotObject, const std::string& slotId,
                ConnectionType connectionType);
        virtual ~GeneralizedConnection() = default;
        EObject *mSignalObject, *mSlotObject;
        const std::string mSignalId, mSlotId;
        const ConnectionType mConnectionType;
    };

    template<typename... ArgTypes>
    struct Connection : public GeneralizedConnection
    {
        template<typename SignalObjectType, typename SlotObjectType>
        Connection(
                SignalObjectType *signalObject,
                const std::string& signalId,
                void (SignalObjectType::*signal)(ArgTypes...),
                SlotObjectType *slotObject,
                const std::string& slotId,
                void (SlotObjectType::*slot)(ArgTypes...),
                ConnectionType connectionType)
                : GeneralizedConnection(signalObject, signalId, slotObject, slotId, connectionType)
        {
            mSignalObject = signalObject;
            mSlotObject = slotObject;
            mSlotCaller = [=](ArgTypes... args){ (slotObject->*slot)(args...); }; // copy capture since slotObject will go out of scope
        }

        std::function<void(ArgTypes...)> mSlotCaller;
    };

};
#endif //ETHREAD2_ECONNECTION_H