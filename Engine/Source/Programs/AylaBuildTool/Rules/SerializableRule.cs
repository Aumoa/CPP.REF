// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Exceptions;

using System.Collections;
using System.Reflection;

namespace AE.Rules;

public abstract class SerializableRule
{
    const BindingFlags Flags = BindingFlags.Public | BindingFlags.Instance
            | BindingFlags.GetProperty | BindingFlags.SetProperty | BindingFlags.DeclaredOnly;

    public virtual void SerializeTo(BinaryWriter Writer)
    {
        for (Type? Cur = GetType(); Cur != null; Cur = Cur.BaseType)
        {
            SerializeToDeclaredType(Writer, Cur);
        }
    }

    protected void SerializeToDeclaredType(BinaryWriter Writer, Type DeclaredType)
    {
        foreach (var Prop in DeclaredType.GetProperties(Flags))
        {
            SerializeToSingleProperty(Writer, DeclaredType, Prop);
        }
    }

    protected void SerializeToSingleProperty(BinaryWriter Writer, Type DeclaredType, PropertyInfo Prop)
    {
        object? Value = Prop.GetValue(this);
        if (Value == null)
        {
            throw new TerminateException(1, CoreStrings.Errors.NullReferenceInRule, Prop.Name, DeclaredType.Name);
        }

        Type PropertyType = Value.GetType();
        if (Value is IList GenericList)
        {
            WriteTo(GenericList.Count, Writer, DeclaredType, Prop);
            foreach (var Item in GenericList)
            {
                WriteTo(Item, Writer, DeclaredType, Prop);
            }
        }
        else
        {
            WriteTo(Value, Writer, DeclaredType, Prop);
        }
    }

    private static void WriteTo(object Value, BinaryWriter Writer, Type DeclaredType, PropertyInfo Prop)
    {
        switch (Value)
        {
            case bool V:
                Writer.Write(V);
                break;
            case char V:
                Writer.Write(V);
                break;
            case byte V:
                Writer.Write(V);
                break;
            case ushort V:
                Writer.Write(V);
                break;
            case uint V:
                Writer.Write(V);
                break;
            case ulong V:
                Writer.Write(V);
                break;
            case sbyte V:
                Writer.Write(V);
                break;
            case short V:
                Writer.Write(V);
                break;
            case int V:
                Writer.Write(V);
                break;
            case long V:
                Writer.Write(V);
                break;
            case float V:
                Writer.Write(V);
                break;
            case double V:
                Writer.Write(V);
                break;
            case decimal V:
                Writer.Write(V);
                break;
            case DateTime V:
                Writer.Write(V.Ticks);
                break;
            case string V:
                Writer.Write(V);
                break;
            case SerializableRule V:
                V.SerializeTo(Writer);
                break;
            default:
                if (Value.GetType().IsEnum)
                {
                    Writer.Write((int)Value);
                    break;
                }
                throw new TerminateException(1, CoreStrings.Errors.NotSupportedType, Prop.Name, DeclaredType.Name);
        }
    }

    public virtual void DeserializeFrom(BinaryReader Reader)
    {
        for (Type? Cur = GetType(); Cur != null; Cur = Cur.BaseType)
        {
            DeserializeFromDeclaredType(Reader, Cur);
        }
    }

    protected void DeserializeFromDeclaredType(BinaryReader Reader, Type DeclaredType)
    {
        foreach (var Prop in DeclaredType.GetProperties(Flags))
        {
            DeserializeFromSingleProperty(Reader, DeclaredType, Prop);
        }
    }

    protected void DeserializeFromSingleProperty(BinaryReader Reader, Type DeclaredType, PropertyInfo Prop)
    {
        if (Prop.PropertyType.IsGenericType && Prop.PropertyType.GetGenericTypeDefinition() == typeof(List<>))
        {
            Type GenericListType = Prop.PropertyType.GetGenericTypeDefinition();
            if (GenericListType != typeof(List<>) && GenericListType != typeof(IList<>))
            {
                throw new TerminateException(1, CoreStrings.Errors.NotSupportedArchitecture, Prop.Name, DeclaredType.Name);
            }

            Type GenericType = Prop.PropertyType.GetGenericArguments()[0];
            ConstructorInfo Ctor = Prop.PropertyType.GetConstructor(Array.Empty<Type>())!;
            var GenericList = (IList)Ctor.Invoke(Array.Empty<object>());

            int Count = (int)ReadFrom(Reader, typeof(int), DeclaredType, Prop);
            for (int i = 0; i < Count; ++i)
            {
                GenericList.Add(ReadFrom(Reader, GenericType, DeclaredType, Prop));
            }
        }
        else
        {
            Prop.SetValue(this, ReadFrom(Reader, Prop.PropertyType, DeclaredType, Prop));
        }
    }

    private static object ReadFrom(BinaryReader Reader, Type GenericType, Type DeclaredType, PropertyInfo Prop)
    {
        switch (Type.GetTypeCode(GenericType))
        {
            case TypeCode.Boolean:
                return Reader.ReadBoolean();
            case TypeCode.Char:
                return Reader.ReadChar();
            case TypeCode.Byte:
                return Reader.ReadByte();
            case TypeCode.UInt16:
                return Reader.ReadUInt16();
            case TypeCode.UInt32:
                return Reader.ReadUInt32();
            case TypeCode.UInt64:
                return Reader.ReadUInt64();
            case TypeCode.SByte:
                return Reader.ReadSByte();
            case TypeCode.Int16:
                return Reader.ReadInt16();
            case TypeCode.Int32:
                return Reader.ReadInt32();
            case TypeCode.Int64:
                return Reader.ReadInt64();
            case TypeCode.Single:
                return Reader.ReadSingle();
            case TypeCode.Double:
                return Reader.ReadDouble();
            case TypeCode.Decimal:
                return Reader.ReadDecimal();
            case TypeCode.DateTime:
                long Ticks = Reader.ReadInt64();
                return new DateTime(Ticks);
            case TypeCode.String:
                return Reader.ReadString();
            default:
                if (GenericType.IsEnum)
                {
                    return Reader.ReadInt32();
                }
                else
                {
                    ConstructorInfo? Ctor = GenericType.GetConstructor(Array.Empty<Type>());
                    if (Ctor == null)
                    {
                        throw new TerminateException(1, CoreStrings.Errors.NotSupportedType, Prop.Name, DeclaredType.Name);
                    }

                    if (Ctor.Invoke(Array.Empty<object>()) is not SerializableRule V)
                    {
                        throw new TerminateException(1, CoreStrings.Errors.NotSupportedType, Prop.Name, DeclaredType.Name);
                    }

                    V.DeserializeFrom(Reader);
                    return V;
                }
        }
    }
}
