

import Realm, { BSON, ObjectSchema } from 'realm';

/**
 * The `Task` data model.
 */
export class Users extends Realm.Object {
    _id!: BSON.ObjectId;
    name!: string;
    userId!: string;

    static schema: ObjectSchema = {
        name: 'Users',
        primaryKey: '_id',
        properties: {
            _id: 'objectId',
            name: 'string',
            userId: 'string',
        },
    };
}
