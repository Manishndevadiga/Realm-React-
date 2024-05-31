// src/hooks/useUserManager.ts
import { useCallback, useEffect, useState } from 'react';
import { useRealm, useUser } from '@realm/react';
import { Users } from '../models/Users';
import { ObjectId } from 'bson'; // Import ObjectId from bson

export function useUserManager() {
    const realm = useRealm();
    const user = useUser();
    const [requeryFlag, setRequeryFlag] = useState(false);
    const users = realm.objects(Users); // Query the Users collection

    useEffect(() => {
        setRequeryFlag(true);
    }, []);

    const addUser = useCallback((name: string, userId: string) => {
        realm.write(() => {
            realm.create(Users, { _id: new ObjectId(), name, userId }); // Ensure ObjectId is created correctly
        });
    }, [realm]);

    const deleteUser = useCallback((user: Users) => {
        realm.write(() => {
            realm.delete(user);
        });
    }, [realm]);

    return {
        users,
        addUser,
        deleteUser,
    };
}
